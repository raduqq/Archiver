#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

#define RECORDSIZE 512
#define LINE_MAX 512

int main() {
//purpose: parsing files, usermap
    char buffer[LINE_MAX];
    union record filedata;

    char directory_name[] = {"testradu/"};
    char files_path[LINE_MAX], usermap_path[LINE_MAX];

    strcpy(filedata.header.magic, "GNUtar ");
    memset(&filedata, '\0', sizeof(filedata));

    strcpy(files_path, directory_name);
    strcpy(usermap_path, directory_name);

    strcat(files_path, "files.txt");
    strcat(usermap_path, "usermap.txt");

    FILE *files = fopen(files_path, "r");
    FILE *usermap = fopen(usermap_path, "r");
    

    while(fgets(buffer, sizeof(buffer), files)) {
        memset(filedata.header.chksum, '\0', sizeof(filedata.header.chksum));
        //TODO: mod, user/groupname, size, timestamp, name/linkname
        
    }

    fclose(files);
    fclose(usermap);
    
    return 0;
}
