// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tema3.h"

void create(char archive_name[], char directory_name[]) {
    int i, j;
    char buffer[RECORDSIZE], files_path[RECORDSIZE], usermap_path[RECORDSIZE];
    union record filedata;

    strcpy(filedata.header.magic, "GNUtar ");
    memset(&filedata, '\0', sizeof(filedata));

    strcpy(files_path, directory_name);
    strcpy(usermap_path, directory_name);

    strcat(files_path, "files.txt");
    strcat(usermap_path, "usermap.txt");

    FILE *files = fopen("files.txt", "r");
    FILE *usermap = fopen("usermap.txt", "r");

    while(fgets(buffer, sizeof(buffer), files)) {
        memset(filedata.header.chksum, '\0', sizeof(filedata.header.chksum));
        ///obs : buffer ends in NEW LINE
    }

    fclose(files);
    fclose(usermap);
}