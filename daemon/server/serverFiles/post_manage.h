#ifndef FILE_MANAGE
#define FILE_MANAGE

#include "reqInfo.h"

void post_manage(char * contentSizeP, char buffer[1025], ReqInfo reqData, char logFileName[256]);
void getTimeName(char fname [50]);
int newName(char buffer [1025], char fname [50], char fullname [256]);
size_t writeFile(FILE* fp, char buffer[1025], ReqInfo postData);


#endif