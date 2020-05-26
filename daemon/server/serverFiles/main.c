// Server side C program to demonstrate HTTP Server programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "configHandler.h"
#include "reqInfo.h"
#include "post_manage.h"
/*
Paginas usadas:
https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
https://stackoverflow.com/questions/30440188/sending-files-from-client-to-server-using-sockets-in-c 
*/



void handleConnections(int new_socket, int server_fd, int addrlen, struct sockaddr_in address);


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


void handleConnections(int new_socket, int server_fd, int addrlen, struct sockaddr_in address  ){
    
    // Only this line has been changed. Everything is same.
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nPost an image to this direction to filter and classify it!";
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
        
        ReqInfo reqData = {.new_socket=new_socket, .b=b, .headerSize=headerSize, .contentSize=0, .dataStart=dataStart} ;
        
        if ((strstr(buffer,"POST") != NULL) && (contentSizeP != NULL)){                                    
            post_manage(contentSizeP, buffer, reqData, logFileName );
        }else if (strstr(buffer,"GET") != NULL){
            1;
        }                
        
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------");
        close(new_socket);
    }
}

