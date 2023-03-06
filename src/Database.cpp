#include <algorithm>
#include <list>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "Database.h"
#include "AVL.h"

bool sst_filename_compare(std::string file_a, std::string file_b) {
  int file_a_age = std::stoi(file_a.substr(4));
  int file_b_age = std::stoi(file_b.substr(4));
  return file_a_age < file_b_age;
}

Database::Database (long memtable_size) : memtable_size(memtable_size){
  db_open = false;
  // REMOVE After testing
  memtable = new AVL_Tree();
  storage = new Storage(name);
}

void Database::open(std::string db_name) {
  // TODO
  // Read directory
  if (db_open) {
    std::string err_msg = "Please close the " + name + " database first";
    perror(err_msg.c_str());
    return;
  }
  db_open = true;
  name = db_name;

  // TODO: Create a directory with name db_name if it already does not exist
  struct stat dir_info;
  const char *db_name_chr = db_name.c_str();

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

        std::sort(sst_files.begin(), sst_files.end(), sst_filename_compare);

        storage = new Storage(name, sst_files);

    } else {
        // Dir doesn't exist, create dir
        if (mkdir(db_name_chr, 0777) != 0) {
          perror("Couldn't create directory");
          return;
        } else {
          storage = new Storage(name);
        }
    }



  // TODO: Use different Storage constructors based on if SSTs already exist or not

  memtable = new AVL_Tree();
}

void Database::put(long key, long value) {
  // TODO

  // If sizeof(key) + memtable->size  > memtable_size, 
  //   call range_scan
  //   create SST
  //   clear memtable (AVL Tree)
  // Insert key, value into memtable
  
}

long Database::get(long key) {
  // TODO

  
}

std::vector<std::pair<long, long>> Database::scan(long key1, long key2) {
  // TODO
}

void Database::close() {
  // TODO
}