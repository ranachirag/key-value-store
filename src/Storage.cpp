#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "Storage.h"
#include "SST.h"

int Storage::create_SST(std::list<std::pair<long int, long int>> data) {
  char filename[] = "test.bin";
  
  int fd = open(filename, O_WRONLY | O_DIRECT); // Add O_DIRECT Flag

  printf("fd = %d\n", fd);

  if (fd ==-1) {
      perror("Error opening file");
  }

  int nbytes = 0;
  int offset = 0;

  // TODO: Write BLOCK_SIZE (4KB) bytes to file
  // TODO: How to convert std::list into a buffer?

  for (auto const i : data) {
    nbytes = pwrite(fd, &(i.first), sizeof(long int), offset);
    offset += sizeof(long int);
    nbytes = pwrite(fd, &(i.second), sizeof(long int), offset);
    offset += sizeof(long int);
  }

  // TODO: Add file name to list of SSTs in this storage class

  close(fd);

  return 0;

}

SST Storage::open_SST() {
  char filename[] = "test.bin";
  
  int fd = open(filename, O_RDONLY | O_DIRECT); // Add O_DIRECT Flag

  if (fd ==-1) {
      perror("Error opening file");
  }

  // TODO: Read File
  

  close(fd);


}


