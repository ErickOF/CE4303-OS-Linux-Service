#ifndef GET_IMG_SIZE
#define GET_IMG_SIZE
#include <stdint.h>

uint32_t reverse_bytes(uint32_t bytes);


#define readbyte(a,b) do if(((a)=getc((b))) == EOF) return 0; while (0)
#define readword(a,b) do { int cc_=0,dd_=0; \
                          if((cc_=getc((b))) == EOF \
        		  || (dd_=getc((b))) == EOF) return 0; \
                          (a) = (cc_<<8) + (dd_); \
                          } while(0)


int scanhead_JPEG (FILE * infile, uint32_t * image_width, uint32_t * image_height);

int scanhead_PNG(FILE * fp, uint32_t * image_width, uint32_t * image_height);

int getImageSize(char fname[256], uint32_t * image_width, uint32_t * image_height);


#endif