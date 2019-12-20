// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tema3.h"

int to_octal(int x) {
    int rest, octal = 0, reverse = 0;

    while(x) {
        rest = x - ((x / 8) * 8);
        octal = octal * 10 + rest;
        x /= 8;
    }

    while(octal) {
        reverse = reverse * 10 + (octal % 10);
        octal /= 10;
    }
    return reverse;
}

void create(char archive_name[], char directory_name[]) {
    int i, j;
    char buffer[LINE_MAX], files_path[LINE_MAX], usermap_path[LINE_MAX];
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