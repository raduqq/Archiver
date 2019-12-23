// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

void list(char *archive_name) {
    int i, j, filesize;        
    union record filedata;
    char buffer[RECORDSIZE], name[sizeof(filedata.header.name)], filesize_aux[sizeof(filedata.header.size)]; 
        
    FILE *archive;

    archive = fopen(archive_name, "rb");
    opened_file_check(archive);

    fseek(archive, 0, SEEK_END);
    int eof_pos = ftell(archive);
    fseek(archive, 0, SEEK_SET);

//    while(ftell(archive) < eof_pos) {
        // reading the header:
        // name
        int pos_init = ftell(archive);
        fread(name, sizeof(name), 1, archive);
        printf("> %s\n", name);
        
        // size
        int filesize_position = sizeof(filedata.header.mode) + sizeof(filedata.header.uid) + sizeof(filedata.header.gid);
        
        fseek(archive, filesize_position, SEEK_CUR);
        fread(filesize_aux, sizeof(filesize_aux), 1, archive);
        printf("Initially.. ");
        puts(filesize_aux);
        
        int ok = 0;
        for (i = 0, j = 0; i < sizeof(filesize_aux); i++) { //something s getting funky right here
            if(filesize_aux[i]) {
                ok = 1;
            }
            if(ok) {
                filesize_aux[j] = filesize_aux[i];
                j++;
            }
            filesize_aux[i] = '\0';
        }
        sscanf(filesize_aux, "%d", &filesize);
        printf("Size = %d\n", filesize);
        puts(filesize_aux);
        // TODO: to decimal

        // skipping to content
        int pos_cur = ftell(archive);
        fseek(archive, RECORDSIZE - (pos_cur - pos_init), SEEK_CUR);

        // content


        // checks if end of archive (marked by a record of zeroes) is reached    
        /*if(ftell(archive) + RECORDSIZE == eof_pos) {
            break;
        }*/
  //  }
    printf("< Done!\n");
}