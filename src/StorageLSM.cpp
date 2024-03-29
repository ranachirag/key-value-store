#include "Storage.h"
#include "utils.h"
#include <climits>
#include <cstring>
#include <iostream>


int flush_buffer(SST *sst, long *buffer, int buf_size) {
  std::vector<std::pair<long, long>> data;
  
  for (int i = 0; i < buf_size; i += 2) {
    data.emplace_back(buffer[i], buffer[i + 1]);
  }
  sst->write_to_file(data);

  std::memset(buffer, 0, buf_size * sizeof(long));
}

// ---------------- LSM Storage Tree Level -----------------------------

std::string LevelLSM::level_LSM_filepath(int level_num, int sst_num) {
  std::string sst_filename = "SST_" + std::to_string(sst_num) + "_" + std::to_string(level_num) + ".bin";
  std::string filepath = options.db_name + "/" + sst_filename;
  return filepath;
}

LevelLSM::LevelLSM(LevelLSMOptions options): options(options), SSTs(0){
  next_level = nullptr;
}

int LevelLSM::num_ssts() {
  return SSTs.size();
}

int LevelLSM::compact_SSTs() {
  // Compact level only if meeting the threshold
  if(SSTs.size() != SIZE_RATIO) {
    return -1;
  }
  
  // Determine if current level is the last one in LSM Tree 
  bool last_level = false;
  if(next_level == nullptr) {
    last_level = true;
  }

  int error_code;

  // Allocate Compaction buffers
  void *old_sst_blk;
  error_code = posix_memalign(&old_sst_blk, BLOCK_SIZE, BLOCK_SIZE);
  if(error_code != 0) {
    perror("Could not allocate buffer");
    return -1;
  }

  void *new_sst_blk;
  error_code = posix_memalign(&new_sst_blk, BLOCK_SIZE, BLOCK_SIZE);
  if(error_code != 0) {
    perror("Could not allocate buffer");
    return -1;
  }

  void *result;
  error_code = posix_memalign(&result, BLOCK_SIZE, BLOCK_SIZE);
  if(error_code != 0) {
    perror("Could not allocate buffer");
    return -1;
  }


  long *result_long_arr = (long *) result;
  int result_buf_size = BLOCK_SIZE / sizeof(long);
  int result_buf_index = 0;

  if(last_level) {
    // Create new level
    LevelLSMOptions level_LSM_options = options_utils::level_to_next_level(options, SIZE_RATIO);
    next_level = new LevelLSM(level_LSM_options);
  }

  // Merge SSTs
  int sst_num = next_level->num_ssts() + 1;
  std::string filepath = level_LSM_filepath(options.level_num + 1, sst_num);

  SSTOptions sst_options = options_utils::level_to_sst(options, filepath, options.sst_capacity * 2, false);

  SST *merged_sst = new ListSST(sst_options);

  // Merge data
  bool merged = false;

  SST *old_sst_obj = SSTs[0];
  SST *new_sst_obj = SSTs[1];

  int old_blk_num = 0;
  int new_blk_num = 0;

  int old_sst_num_entries = old_sst_obj->read_block(old_sst_blk, old_blk_num);
  int new_sst_num_entries = new_sst_obj->read_block(new_sst_blk, new_blk_num);

  bool old_sst_reached_end = false;
  bool new_sst_reached_end = false; 

  std::vector<std::pair<long, long>> old_vals{};
  if(old_sst_num_entries > 0) {
    old_vals = file_utils::reintrepret_buffer(old_sst_blk, old_sst_num_entries * (KEY_SIZE + VALUE_SIZE));
  } else {
    old_sst_reached_end = true;
  }

  std::vector<std::pair<long, long>> new_vals{};
  if(new_sst_num_entries > 0) {
    new_vals = file_utils::reintrepret_buffer(new_sst_blk, new_sst_num_entries * (KEY_SIZE + VALUE_SIZE));
  } else {
    new_sst_reached_end = true;
  }

  int i = 0;
  int j = 0;


  while (!old_sst_reached_end || !new_sst_reached_end) {
    std::pair<long, long> entry_to_insert;

    int old_sst_key;
    if(!old_sst_reached_end) {
      old_sst_key = old_vals[i].first;
    }

    int new_sst_key;
    if(!new_sst_reached_end) {
      new_sst_key = new_vals[j].first;
    }

    if(!new_sst_reached_end && !old_sst_reached_end) {
      entry_to_insert = new_vals[j];
      if(old_sst_key < new_sst_key) {
        entry_to_insert = old_vals[i];
        i++;
      } else if(old_sst_key == new_sst_key) {
        // Same key, insert entry from newer SST, skip over older entry
        i++;
        j++;
      } else {
        j++;
      }
    } else if(!new_sst_reached_end) {
      entry_to_insert = new_vals[j];
      j++;
    } else if(!old_sst_reached_end) {
      entry_to_insert = old_vals[i];
      i++;
    }

    // If the merged SST goes into the last level, eliminate tombstones
    if(!(last_level && (entry_to_insert.second == LONG_MIN))) {
      
      if(result_buf_index == result_buf_size){
        // flush Buffer to SST
        flush_buffer(merged_sst, result_long_arr, result_buf_index);
        result_buf_index = 0;
      }
      result_long_arr[result_buf_index] = entry_to_insert.first;
      result_buf_index++;
      result_long_arr[result_buf_index] = entry_to_insert.second;
      result_buf_index++;
    }


    if (i == old_vals.size()) {
      // Reached end of block, read in last block
      old_blk_num++;
      old_sst_num_entries = old_sst_obj->read_block(old_sst_blk, old_blk_num);
      i = 0;
      if(old_sst_num_entries > 0) {
        old_vals = file_utils::reintrepret_buffer(old_sst_blk, old_sst_num_entries * (KEY_SIZE + VALUE_SIZE));
      } else {
        old_sst_reached_end = true;
      }
    }

    if (j == new_vals.size()) {
      // Reached end of block, read in last block
      new_blk_num++;
      new_sst_num_entries = new_sst_obj->read_block(new_sst_blk, new_blk_num);
      j = 0;
      if(new_sst_num_entries > 0) {
        new_vals = file_utils::reintrepret_buffer(new_sst_blk, new_sst_num_entries * (KEY_SIZE + VALUE_SIZE));
      } else {
        new_sst_reached_end = true;
      }
    }
  }

  // flush any remaining data in the Buffer to SST 
  if(result_buf_index > 0){
    flush_buffer(merged_sst, result_long_arr, result_buf_index);
    result_buf_index = 0;
  }

  // Deallocate compaction buffers
  free(old_sst_blk);
  free(new_sst_blk);
  free(result);

  // Add SST to next level
  next_level->add_sst(merged_sst);

  // Remove SSTs at current level, as they are merged
  old_sst_obj->delete_sst_file();
  new_sst_obj->delete_sst_file();

  SSTs.clear();
  SSTs.resize(0);

  return 0;
}

int LevelLSM::add_sst(std::vector<std::pair<long, long>> &data) {

  std::string filepath = level_LSM_filepath(options.level_num, num_ssts() + 1);

  SSTOptions sst_options = options_utils::level_to_sst(options, filepath, options.sst_capacity, false);

  SST *sst_to_add;
  if(options.sst_structure == LIST_SST) {
    sst_to_add = new ListSST(sst_options);
  } 
  SSTs.emplace_back(sst_to_add);
  sst_to_add->write_to_file(data);

  // Check for compaction
  compact_SSTs();
}

int LevelLSM::add_sst(SST *sst) {
  SSTs.emplace_back(sst);
  compact_SSTs(); 
}

int LevelLSM::initialize_level(std::string sst_filename) {
  int file_level = std::stoi(sst_filename.substr(6));

  if(file_level == options.level_num) {
    std::string filepath = options.db_name + "/" + sst_filename;
    SSTOptions sst_options = options_utils::level_to_sst(options, filepath, options.sst_capacity, true);

    SST *sst_to_add;
    if(options.sst_structure == LIST_SST) {
      sst_to_add = new ListSST(sst_options);
    } 

    SSTs.emplace_back(sst_to_add);
    compact_SSTs(); 
  } else {
    if(next_level == nullptr) {
      LevelLSMOptions level_LSM_options = options_utils::level_to_next_level(options, SIZE_RATIO);
      next_level = new LevelLSM(level_LSM_options);
    }
    next_level->initialize_level(sst_filename);
  }
  
}

long LevelLSM::get_value(long key, bool &val_found) {
  long val;
  for (SST *sst: SSTs) {
    val = sst->search(key, val_found);
    if(val_found) {
      if(val == LONG_MIN) {
        val_found = false;
        return -1;
      }
      return val;
    }
  }

  if (next_level) {
    return next_level->get_value(key, val_found);
  }
  
  val_found = false;
  return -1;
}

SST *LevelLSM::get_sst() {
  if (num_ssts() > 0) {
    return SSTs[0];
  }
  return nullptr;
}

void LevelLSM::update_bloom_filter_param(int bits_per_entry) {
  if(options.use_bloom_filters) {
    options.bloom_filter_options.bits_per_entry = bits_per_entry;
  }
}

// ---------------- LSM Storage Tree -----------------------------

StorageLSM::StorageLSM(StorageOptions options) : options(options) {
  LevelLSMOptions level_LSM_options = options_utils::storage_to_level(options, 1);
  first_level = new LevelLSM(level_LSM_options);
}

StorageLSM::StorageLSM(StorageOptions options, std::vector<std::string> sst_files) : options(options) {
  LevelLSMOptions level_LSM_options = options_utils::storage_to_level(options, 1);
  first_level = new LevelLSM(level_LSM_options);

  for(std::string sst_file: sst_files) {
    first_level->initialize_level(sst_file);
  }
}

void StorageLSM::add_to_storage(std::vector<std::pair<long, long>> &data) {
  first_level->add_sst(data);
}

long StorageLSM::get_value(long key, bool &val_found) {
  return first_level->get_value(key, val_found);
}


int StorageLSM::scan_storage(std::vector<std::pair<long, long> > &result, long key1, long key2) {
  std::vector<void *> buffs;
  std::vector<SST *> level_ssts;
  std::vector<int> indices;
  std::vector<int> buff_sizes;
  std::vector<int> block_nums;

  LevelLSM *curr = first_level;
  while(curr != nullptr) {
    if(curr->num_ssts() > 0) {
      
      void *buf_blk;
      int error_code = posix_memalign(&buf_blk, BLOCK_SIZE, BLOCK_SIZE);
      if(error_code != 0) {
        perror("Could not allocate buffer");
        return -1;
      }

      SST *sst_to_read = curr->get_sst();
      int num_entries = sst_to_read->read_block(buf_blk, 0);

      block_nums.push_back(0);
      level_ssts.push_back(sst_to_read);
      buff_sizes.push_back(num_entries * (KEY_SIZE + VALUE_SIZE));
      buffs.push_back(buf_blk);
      indices.push_back(0);
    }
    curr = curr->next_level;
  }
  int num_found = 0;
  int counter = key1;

  while (counter <= key2) {
    bool found_counter = false;

    // Check all levels for counter 
    for(int i = 0; i < buffs.size(); ++i) {
      
      bool stop_counter = false;
      while (!stop_counter) {
        int index = indices[i];
        if(index == -1) {
          break;
        }
        void *buffer = buffs[i];
        int buffer_bytes = buff_sizes[i];
        SST *sst = level_ssts[i];
        std::vector<std::pair<long, long>> key_values = file_utils::reintrepret_buffer(buffer, buffer_bytes);
        
        std::pair<long, long> entry_to_check = key_values[index];

        if(entry_to_check.first > counter) {
          stop_counter = true;
        } else if(entry_to_check.first == counter) {
          
          if(entry_to_check.second != LONG_MIN) {
            result.push_back(entry_to_check);
            num_found++;
          }

          found_counter = true;
          indices[i]++;
        } else {
          indices[i]++;
        }

        if(index == key_values.size()) {
          block_nums[i]++;
          int num_entries_read = sst->read_block(buffer, block_nums[i]);
          if(num_entries_read > 0) {
            indices[i] = 0;
          } else {
            // Reached end of SST, no more checking
            indices[i] = -1;
            break;
          }
        }

      }

       if(found_counter) {
        // Found counter, no need to check later levels as current level would have latest value
        break;
      }
    }
    counter++;
  }

  for(int i = 0; i < buffs.size(); ++i) {
    void *buf = buffs[i];
    free(buf);
    buffs[i] = nullptr;
  }
  return num_found;
}

void StorageLSM::reset() {
  LevelLSM *curr = first_level;
  while (curr != nullptr) {
    LevelLSM *level_to_delete = curr;
    curr = curr->next_level;

    SST *sst_to_delete = level_to_delete->get_sst();
    delete sst_to_delete;
    delete level_to_delete;
  }
}

void StorageLSM::update_bloom_filter_param(int bits_per_entry) {
  if(options.use_bloom_filters) {
    options.bloom_filter_options.bits_per_entry = bits_per_entry;
    LevelLSM *curr = first_level;
    while (curr != nullptr) {
      curr->update_bloom_filter_param(bits_per_entry);
      curr = curr->next_level;
    }
  }
}