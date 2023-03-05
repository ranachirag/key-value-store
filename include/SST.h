#ifndef SST_H
#define SST_H

class SST {
  public: 
    int method;
    virtual long int search(long int key);
};

class SST_Array : public SST {
  public:
    virtual long int search(long int key);
};


class SST_BTree : public SST {
  public:
    virtual long int search(long int key);
};


#endif