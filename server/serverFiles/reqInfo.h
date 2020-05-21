#ifndef REQUES_INFO
#define REQUES_INFO


typedef struct ReqInfo{
  int new_socket;
  size_t b;
  size_t headerSize;
  size_t contentSize;
  char * dataStart;
} ReqInfo;



#endif