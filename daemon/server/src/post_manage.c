#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <stdint.h>

#include "../include/configHandler.h"
#include "../include/post_manage.h"
#include "../include/reqInfo.h"
#include "../include/configHandler.h"
#include "../include/getImageSize.h"

#include "../include/ImageProcessing.h"
#include "../include/constants.h"

uint32_t WIDTH = 235;
uint32_t HEIGHT = 215;
uint32_t CHANNELS = 3;

void post_manage(char * contentSizeP, char buffer[1025], ReqInfo reqData, char logFileName[256] ){
    FILE* fpLog;

    contentSizeP += 15; //Size of "Content-Length:"
    size_t contentSize =  strtol(contentSizeP, NULL, 10); //Converts to size_t the size
    if (contentSize == 0){
        printf("Content size was 0\n");
        return;
    }
    reqData.contentSize=contentSize;
                
    char fname[50] = ""; // Filename
    char fullname [256] = "";              
    newName(buffer, fname, fullname);
    FILE* fp = fopen( fullname, "wb");                                    
    size_t totWritten = writeFile(fp,  buffer, reqData);
    
    int strongestColor = -1;
    char filteredName[256];

    fpLog = fopen(logFileName, "a+");
    fprintf(fpLog, "Received: %ld, Header size: %ld, Content size: %ld\n", totWritten+reqData.headerSize, reqData.headerSize, contentSize );            
    fprintf(fpLog, "Saved in: %s\n", fullname);
    fclose(fpLog);  

    printf("Received: %ld, Header size: %ld, Content size: %ld\n", totWritten+reqData.headerSize, reqData.headerSize, contentSize );            
    printf("Saved in: %s\n", fullname);


    if (totWritten == contentSize){        
        strcpy(filteredName, dirHis);
        strcat(filteredName, fname);

        getImageSize(fullname, &WIDTH, &HEIGHT);
        
        processMSG(logFileName);

        strongestColor = processImage( fullname , filteredName, filters, kmedian, kavg );

        char command[512] = "mv ";
        strcat(command,fullname);
        strcat(command," ");
        strcpy(fullname,dirCol);
        if (strongestColor == 0){
            strcat(fullname,"/Red/");
        }else if (strongestColor == 1){
            strcat(fullname,"/Green/");
        }else if (strongestColor == 2){
            strcat(fullname,"/Blue/");
        }
        strcat(fullname,fname);
        strcat(command,fullname);
        system(command);    
    }

    
    fpLog = fopen(logFileName, "a+");
    fprintf(fpLog, "Filtered Image saved in: %s\n", filteredName );            
    fprintf(fpLog, "Image moved to: %s\n\n", fullname);
    fclose(fpLog);  

    printf("Filtered Image saved in: %s\n", filteredName );            
    printf("Image moved to: %s\n\n", fullname);
    
}

void processMSG(char logFileName[256]){
    FILE* fpLog = fopen(logFileName, "a+");


    printf("Image size: %d, %d\n", WIDTH, HEIGHT);
    fprintf(fpLog,"Image size: %d, %d\n", WIDTH, HEIGHT);

    
    if (filters == 1){
        printf("Using median filter of window size %d\n",kmedian);
        fprintf(fpLog, "Using median filter of window size %d\n",kmedian);
        
    }else if(filters == 2){
        printf("Using average filter of window size %d\n",kavg);
        fprintf(fpLog, "Using average filter of window size %d\n",kavg);
 
     }else if(filters == 3){
        printf("Using median filter of window size %d and then ",kmedian);
        fprintf(fpLog, "Using median filter of window size %d and then",kmedian);
        printf("using average filter of window size %d\n",kavg);
        fprintf(fpLog, "using average filter of window size %d\n",kavg);
        

    }else if(filters == 4){    
        printf("Using average filter of window size %d and then ",kavg);
        fprintf(fpLog, "Using average filter of window size %d and then",kavg);
        printf("using median filter of window size %d\n",kmedian);
        fprintf(fpLog, "using median filter of window size %d\n",kmedian);

    }else{
        printf("Using median filter of window size %d\n",3);
        fprintf(fpLog, "Using median filter of window size %d\n",3);

    }

    fclose(fpLog);  

}

void getTimeName(char fname [50]){
    time_t rawtime;
    time(&rawtime);
    strftime(fname,49,"%Y-%m-%d-%H-%M-%S", localtime(&rawtime)); //Formats time        
}



int newName(char buffer [1025], char fname [50], char fullname [256]){
    // If the header contains the name for the new file using the format:
    // Filename: example.png
    char * filenameP = strstr(buffer,"Filename");
    if (filenameP != NULL){
        char * filenamePE = strstr(filenameP,"\n"); // Points to the end of this line
        filenameP += 10; //Size of "Filename:"
        int fNameLen = filenamePE - filenameP;
        if (fNameLen > 1){
            memcpy(fname,filenameP,fNameLen);      // Copies name to fname 
        }else{
            filenameP = NULL; //If the given name is too small it uses the time format name below
        }
    } 
    /* If the header doesn't contain the name of the file, the name will be taken 
    * from the current time Y-m-d-H-M-S
    * if the Content-Type header is included, appends the corresponding extension .jpg or .png
    * Content-Type: *png*   OR   Content-Type: *jpeg*
    */
    if(filenameP == NULL) {                
        getTimeName(fname);
        char * contentTypeP = strstr(buffer,"Content-Type:");
        if (contentTypeP != NULL){
            int isJPEG = (strstr(contentTypeP,"jpeg") != NULL)||(strstr(contentTypeP,"jpg") != NULL);
            int isPNG = strstr(contentTypeP,"png") != NULL;
            if (isJPEG){
                strcat(fname, ".jpeg");
            }else if (isPNG){
                strcat(fname, ".png");
            }
        }                             
    }
    // Fullname = dir + filename
    //strcat(fullname, dir);
    //srand(time(NULL));   // Initialization, should only be called once.

     
    strcpy(fullname,dirCol);       
    strcat(fullname, fname); 
    
    return 0;
}





size_t writeFile(FILE* fp, char buffer[1025], ReqInfo pData){
    size_t tot = 0;   //Total bytes of content counter                    
    if (pData.headerSize != pData.b){
        if(fp != NULL  ){                
            fwrite(pData.dataStart, 1, pData.b-pData.headerSize, fp);
            tot += (pData.b-pData.headerSize);                                
            while( tot < pData.contentSize ){
                pData.b = recv(pData.new_socket, buffer, 1024,0) ;
                if (pData.b < 1){
                    printf("\nError reading contents!\n");
                    break; // Handles disconnection from client                            
                }
                tot += pData.b;
                fwrite(buffer, 1, pData.b, fp);            
            }            
            printf("Received bytes: %ld, header size %ld, content size %ld\n",tot+pData.headerSize, pData.headerSize, pData.contentSize);            
            fclose(fp);                                
        }else{
            perror("File\n");
        } 
    }else{
        fclose(fp);   
        printf("No Content Received!\n");
    }    
    return tot;
}


