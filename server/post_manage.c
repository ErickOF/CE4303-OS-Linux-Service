#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>

#include "configHandler.h"
#include "post_manage.h"
#include "reqInfo.h"
#include "configHandler.h"

#include "include/ImageProcessing.h"

void post_manage(char * contentSizeP, char buffer[1025], ReqInfo reqData, char logFileName[256] ){
    FILE* fpLog;

    contentSizeP += 15; //Size of "Content-Length:"
    size_t contentSize =  strtol(contentSizeP, NULL, 10); //Converts to size_t the size
    reqData.contentSize=contentSize;
                
    char fname[50] = ""; // Filename
    char fullname [256] = "";              
    newName(buffer, fname, fullname);
    FILE* fp = fopen( fullname, "wb");                                    
    size_t totWritten = writeFile(fp,  buffer, reqData);

    fpLog = fopen(logFileName, "a+");
    fprintf(fpLog, "Received: %ld, Header size: %ld, Content size: %ld\n", totWritten+reqData.headerSize, reqData.headerSize, contentSize );            
    fprintf(fpLog, "Saved in: %s\n\n", fullname);

    
/*
    

    if (totWritten == contentSize){
        char filteredName[256];
        strcpy(filteredName, dirHis);
        strcat(filteredName, fname);
        
        int strongestColor = processImage( char fullname [256], char filteredName[256] );

        char command[512] = "mv ";
        strcat(command,fullname);
        strcat(command," ");

        strcat(command,dirCol);

        if (strongestColor == 0){
            strcat(command,"/Red/");
        }else if (strongestColor == 1){
            strcat(command,"/Blue/");
        }else if (strongestColor == 2){
            strcat(command,"/Green/");
        }
        strcat(command,fname);
        system(command);

        
    }
*/
    
    // Cerrar el archivo en temporal


    


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
    printf("Saving file in %s\n",fullname);    
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


