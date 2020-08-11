
/* portions derived from IJG code */

#include <stdio.h>
#include <string.h>

#include "../include/getImageSize.h"

uint32_t reverse_bytes(uint32_t bytes)
{
    uint32_t aux = 0;
    uint8_t byte;
    int i;

    for(i = 0; i < 32; i+=8)
    {
        byte = (bytes >> i) & 0xff;
        aux |= byte << (32 - 8 - i);
    }
    return aux;
}


int scanhead_JPEG (FILE * infile, uint32_t * image_width, uint32_t * image_height) {
  int marker=0;
  int dummy=0;
  if ( getc(infile) != 0xFF || getc(infile) != 0xD8 )
    return 0;

  for (;
      ;) {


    int discarded_bytes=0;
    readbyte(marker,infile);
    while (marker != 0xFF) {
      discarded_bytes++;
      readbyte(marker,infile);
    }
    do readbyte(marker,infile); while (marker == 0xFF);

    if (discarded_bytes != 0) return 0;
   
    switch (marker) {
    case 0xC0:
    case 0xC1:
    case 0xC2:
    case 0xC3:
    case 0xC5:
    case 0xC6:
    case 0xC7:
    case 0xC9:
    case 0xCA:
    case 0xCB:
    case 0xCD:
    case 0xCE:
    case 0xCF: {
      readword(dummy,infile);	/* usual parameter length count */
      readbyte(dummy,infile);
      readword((*image_height),infile);
      readword((*image_width),infile);
      readbyte(dummy,infile);

      return 1;
      break;
      }
    case 0xDA:
    case 0xD9:
      return 0;
    default: {
	int length;
	
	readword(length,infile);

	if (length < 2)
	  return 0;
	length -= 2;
	while (length > 0) {
	  readbyte(dummy, infile);
	  length--;
	}
      }
      break;
    }
  }
}


int scanhead_PNG(FILE * fp, uint32_t * image_width, uint32_t * image_height) {
  uint32_t reversedW, reversedH;

  fseek(fp, 16, SEEK_SET);
  fread(&reversedW, 4, 1, fp);

  fseek(fp, 20, SEEK_SET);
  fread(&reversedH, 4, 1, fp);
  *image_width = reverse_bytes(reversedW);
  *image_height = reverse_bytes(reversedH);

  return 0;
}


int getImageSize(char fname[256], uint32_t * image_width, uint32_t * image_height) {
  char * isPNG = strstr(fname,"png");
  char * isJPEG = strstr(fname,"jpeg");
  char * isJPG = strstr(fname,"jpg");
  FILE* fp ;
  if ((isJPEG != NULL)||(isJPG != NULL)){    
    fp = fopen(fname, "rb");
    scanhead_JPEG(fp, image_width, image_height);       
  }else if(isPNG != NULL){        
    fp = fopen(fname, "rb");
    scanhead_PNG(fp, image_width, image_height);    
  }else{
    *image_width = 256;*image_height=256;
  }
  fclose(fp); 
  return 0;
}

