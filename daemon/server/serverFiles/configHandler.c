#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "configHandler.h"

char dirCol[256];
char dirHis[256];
char dirLog[256];
int filters = 0;
int kavg, kmedian;

void readConfig(int* port){
    int c = 0;
    char ch;
    
    FILE *fp = fopen("config.conf","r");    
    if (fp == NULL){
        perror("Cant read file");        
    }    
    
    while((ch = fgetc(fp)) != EOF){
        if (c == 0 && ch == ':'){
            fscanf(fp, "%d", port );
            ++c;
        }else if (c == 1 && ch == ':'){
            fscanf(fp, "%s", dirCol );
            ++c;
        }else if (c == 2 && ch == ':'){
            fscanf(fp, "%s", dirHis );
            ++c;
        }else if (c == 3 && ch == ':'){
            fscanf(fp, "%s", dirLog );
            ++c;
        }else if (c == 4 && ch == ':'){
            fscanf(fp, "%d", &filters );
            ++c;
        }else if (c == 5 && ch == ':'){
            fscanf(fp, "%d", &kavg );
            ++c;
        }else if (c == 6 && ch == ':'){
            fscanf(fp, "%d", &kmedian );
            ++c;
        }else{
            continue;
        }
    }
    fclose(fp);
    
}


void createFolders(){
    char command[300] = "";
    strcpy(command,"mkdir -p "); strcat(command,dirHis);
    system(command);    
    strcpy(command,"mkdir -p "); strcat(command,dirLog);
    system(command);                
    strcpy(command,"mkdir -p "); strcat(command,dirCol); strcat(command,"/Red/");
    system(command);    
    strcpy(command,"mkdir -p "); strcat(command,dirCol); strcat(command,"/Blue/");
    system(command);        
    strcpy(command,"mkdir -p "); strcat(command,dirCol); strcat(command,"/Green/");
    system(command);    
}
