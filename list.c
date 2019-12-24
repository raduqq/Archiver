// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

void list(char *archive_name) {
    unsigned int filesize;        
    union record filedata;
    char buffer[RECORDSIZE], name[sizeof(filedata.header.name)], filesize_aux[sizeof(filedata.header.size)]; 
        
    FILE *archive;

    archive = fopen(archive_name, "rb");
    opened_file_check(archive);

    fseek(archive, 0, SEEK_END);
    int eof_pos = ftell(archive);
    fseek(archive, 0, SEEK_SET);

    while(ftell(archive) < eof_pos) {
        // name
        int pos_init = ftell(archive);
        fread(name, sizeof(name), 1, archive);
        printf("> %s\n", name);
        
        // size
        int filesize_position = sizeof(filedata.header.mode) + sizeof(filedata.header.uid) + sizeof(filedata.header.gid);
        
        fseek(archive, filesize_position, SEEK_CUR);
        fread(filesize_aux, sizeof(filesize_aux), 1, archive);
        
        sscanf(filesize_aux, "%d", &filesize);
        filesize = to_decimal(filesize, OCTAL_BASE);

        // skipping to content
        fseek(archive, RECORDSIZE + pos_init, SEEK_SET);

        // content
        int record_blocks = filesize / RECORDSIZE;
        if (filesize % RECORDSIZE) {
            record_blocks++;
        }
        for(int i = 0; i < record_blocks; i++) {
            fread(buffer, sizeof(buffer), 1, archive);
        }

        // checks if end of archive (marked by a record of zeroes) is reached    
        if(ftell(archive) + RECORDSIZE == eof_pos) {
            break;
        }
   }
   // should be a legit move..
   fclose(archive);
}