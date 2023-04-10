#include <algorithm>
#include <list>
#include <string>
#include <climits>
#include <unistd.h>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "utils.h"
#include "Database.h"
#include "Storage.h"
#include "AVL.h"


Database::Database (DatabaseOptions options) : options(options) {
  db_open = false;
  buffer_pool = nullptr;
}

void Database::open(std::string db_name) {
  // Read directory
  if (db_open) {
    std::string err_msg = "Please close the " + name + " database first";
    perror(err_msg.c_str());
    return;
  }
  db_open = true;
  name = db_name;

  struct stat dir_info;
  const char *db_name_chr = db_name.c_str();

  StorageOptions storage_options;
  storage_options.db_name = name;
  storage_options.use_buffer_pool = options.use_buffer_pool;
  if(options.use_buffer_pool) {
    buffer_pool = new BufferPool(options.buffer_pool_options);
    storage_options.buffer_pool = buffer_pool;
  }
  storage_options.sst_structure = options.sst_structure;
  storage_options.use_bloom_filters = options.use_bloom_filters;
  storage_options.bloom_filter_options = options.bloom_filter_options;
  storage_options.memtable_capacity = options.memtable_size / (VALUE_SIZE + KEY_SIZE); // Memtable size is in terms of bytes

  if (stat(db_name_chr, &dir_info) == 0 && (dir_info.st_mode & S_IFDIR) != 0) {
    // Dir exists, read SSTs
    std::vector<std::string> sst_files;

    DIR* dir = opendir(db_name_chr);
    if (dir == NULL) {
        perror("Couldn't open directory");
        return;
    }
    dirent* dir_file;
    while ((dir_file = readdir(dir)) != NULL) { 
      if (dir_file->d_type == DT_REG) {  
        sst_files.push_back(dir_file->d_name);
      }
    }
    closedir(dir);

    if(options.storage_structure == APPEND_ONLY_STORAGE) {
      std::sort(sst_files.begin(), sst_files.end(), file_utils::sst_filename_compare);
      storage = new StorageAppendOnly(storage_options, sst_files);
    } else if(options.storage_structure == LSM_TREE_STORAGE) {
      storage = new StorageLSM(storage_options, sst_files);
    }
    
  } else {
    // Dir doesn't exist, create dir
    if (mkdir(db_name_chr, 0777) != 0) {
      perror("Couldn't create directory");
      return;
    } else {
      if(options.storage_structure == APPEND_ONLY_STORAGE) {
        storage = new StorageAppendOnly(storage_options);
      } else if(options.storage_structure == LSM_TREE_STORAGE) {
        storage = new StorageLSM(storage_options);
      }
    }
  }
  
  if(options.storage_structure == LSM_TREE_STORAGE) {
    memtable = new AVLTree(true);
  } else if(options.storage_structure == APPEND_ONLY_STORAGE) {
    memtable = new AVLTree(false);
  }

}

void Database::put(long key, long value) {
  if(!db_open) {
    perror("Please open a database first");
    return;
  }

  if(memtable->size >= options.memtable_size) {
    std::vector<std::pair<long, long> > lst;
    int lst_size = memtable->range_search(lst, memtable->min_key, memtable->max_key);
    storage->add_to_storage(lst);
    memtable->reset_tree();
  }
  memtable->insert(key, value);
  
}

// TODO: Error checking for when key doesn't exist, How to handle?
long Database::get(long key) {
  if(!db_open) {
    perror("Please open a database first");
    return -1;
  }

  // Search Main Memory
  bool val_found;
  long val;

  val = memtable->get_value(key, val_found);
  if (val_found) {
    return val;
  }

  // Search Storage
  val = storage->get_value(key, val_found);
  if (val_found) {
    return val;
  }

  return -1;
  
}

int Database::scan(std::vector<std::pair<long, long> > &result, long key1, long key2) {
  if(!db_open) {
    perror("Please open a database first");
    return -1;
  }

  int kv_range_mem_size = 0;
  int kv_range_storage_size = 0;
  
  if(options.storage_structure == APPEND_ONLY_STORAGE) {
    // Range search Main Memory
    kv_range_mem_size = memtable->range_search(result, key1, key2);

    // Range search Storage
    kv_range_storage_size = storage->scan_storage(result, key1, key2);

  } else if(options.storage_structure == LSM_TREE_STORAGE) {

    std::vector<std::pair<long, long> > memtable_result;

    // Range search Main Memory
    kv_range_mem_size = memtable->range_search(memtable_result, key1, key2);

    kv_range_mem_size = 0;
    for(std::pair<long, long> pair: memtable_result) {
      if(pair.second != LONG_MIN) {
        kv_range_mem_size++;
        result.push_back(pair);
      }
    }

    // Range search Storage - sorted
    kv_range_storage_size = storage->scan_storage(result, key1, key2);

    std::sort(result.begin(), result.end(), sort_utils::compare_kv_pair);

  }

  return kv_range_mem_size + kv_range_storage_size;
}

int Database::remove(long key) {
  if(options.storage_structure == LSM_TREE_STORAGE) {
    put(key, LONG_MIN);
    return 0;
  }
  return -1;
}

int Database::update_buffer_pool_size(int new_max_size) {
  if(db_open && options.use_buffer_pool) {
    int code = buffer_pool->update_directory_size(new_max_size);
    if (code == -1) {
      perror("Unable to update buffer pool size");
      return -1;
    }
  }
  return 0;
}

void Database::close() {
  if(db_open) {
    if(memtable->size > 0) {
      std::vector<std::pair<long, long> > lst;
      int lst_size = memtable->range_search(lst, memtable->min_key, memtable->max_key);
      if(lst_size > 0) {
        storage->add_to_storage(lst);
      }
    }
    memtable->reset_tree();
    storage->reset();

    if(options.use_buffer_pool) {
      buffer_pool->reset_buffer_pool();
      delete buffer_pool;
    }
    
    delete memtable;
    delete storage;

    db_open = false;
  }
}


void Database::update_bloom_filter_param(int bits_per_entry) {
  if(options.use_bloom_filters) {
    storage->update_bloom_filter_param(bits_per_entry);
  }
}