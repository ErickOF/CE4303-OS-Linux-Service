// Server side C program to demonstrate HTTP Server programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include "configHandler.h"

/*
Paginas usadas:
https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
https://stackoverflow.com/questions/30440188/sending-files-from-client-to-server-using-sockets-in-c 
*/

typedef struct ReqInfo{
  int new_socket;
  size_t b;
  size_t headerSize;
  size_t contentSize;
  char * dataStart;
} ReqInfo;

int newName(char buffer [1025], char fname [50], char fullname [256]);
size_t writeFile(FILE* fp, char buffer[1025], ReqInfo postData);
void handleConnections(int new_socket, int server_fd, int addrlen, struct sockaddr_in address);

void getTimeName(char fname [50]){
    time_t rawtime;
    time(&rawtime);
    strftime(fname,49,"%Y-%m-%d-%H-%M-%S", localtime(&rawtime)); //Formats time        
}

int main(){
    int port = 1717;
    readConfig(&port);
    createFolders();
    
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("In socket");
        exit(EXIT_FAILURE);
    } 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    
    // Temporal until config file is used        
    //printf("Enter port: ");
    //scanf("%d", &port);
    address.sin_port = htons( port );
    //address.sin_port = htons( PORT );    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
        
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0){
        perror("In listen");
        exit(EXIT_FAILURE);
    }    
    handleConnections(new_socket, server_fd, addrlen, address );    
    return 0;
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

void handleConnections(int new_socket, int server_fd, int addrlen, struct sockaddr_in address  ){
    
    // Only this line has been changed. Everything is same.
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    char logFileName [256];
    char timeName [50];
    getTimeName(timeName);
    strcpy(logFileName,dirLog); strcat(logFileName,timeName);strcat(logFileName,".log");
    FILE* fpLog;

    while(1){
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[1025] = "";        
        size_t  b;        
        b = recv(new_socket, buffer, 1024,0) ;        
        char * dataStart = strstr(buffer,"\r\n\r\n");
        dataStart += 4;          
        size_t headerSize  = dataStart - buffer;        
        fwrite(buffer, 1, headerSize, stdout); // Prints the header of POST-GET - Later used for Logs        

        fpLog = fopen(logFileName, "a+");
        fwrite(buffer, 1, headerSize-2, fpLog);
        fclose(fpLog);

        char * contentSizeP = strstr(buffer,"Content-Length:");
        
        ReqInfo postData = {.new_socket=new_socket, .b=b, .headerSize=headerSize, .contentSize=0, .dataStart=dataStart} ;
        
        if ((strstr(buffer,"POST") != NULL) && (contentSizeP != NULL)){                                    
            contentSizeP += 15; //Size of "Content-Length:"
            size_t contentSize =  strtol(contentSizeP, NULL, 10); //Converts to size_t the size
            postData.contentSize=contentSize;
                        
            char fname[50] = ""; // Filename
            char fullname [256] = "";              
            newName(buffer, fname, fullname);
            FILE* fp = fopen( fullname, "wb");                                    
            size_t tot = writeFile(fp,  buffer, postData);

            fpLog = fopen(logFileName, "a+");
            fprintf(fpLog, "Received: %ld, Header size: %ld, Content size: %ld\n", tot+headerSize, headerSize, contentSize );            
            fprintf(fpLog, "Saved in: %s\n\n", fullname);

            /*

            char filteredName[256];
            strcpy(filteredName, dirHis);
            strcat(filteredName, fname);

            if (tot = header + content){
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

        }else if (strstr(buffer,"GET") != NULL){
            1;
        }                
        
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------");
        close(new_socket);
    }
}

