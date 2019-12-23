// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

void extract(char *file_name, char *archive_name) {
    unsigned int filesize;        
    union record filedata;
    char to_write_path[sizeof(filedata.header.name) + strlen("extracted_")], name[sizeof(filedata.header.name)], filesize_aux[sizeof(filedata.header.size)]; 
    
    int ok_extracted = 0;

    FILE *to_write, *archive;
    archive = fopen(archive_name, "rb");
    if (archive == NULL) {
        printf("> File not found!\n");
        return;
    }

    fseek(archive, 0, SEEK_END);
    int eof_pos = ftell(archive);
    fseek(archive, 0, SEEK_SET);

    while(ftell(archive) < eof_pos && ok_extracted == 0) {
        // name
        int pos_init = ftell(archive);
        fread(name, sizeof(name), 1, archive);

        if(strcmp(name, file_name) == 0) {
            memset(to_write_path, '\0', sizeof(to_write_path));
            strcpy(to_write_path, "extracted_");
            strcat(to_write_path, file_name);

            puts(to_write_path);

            to_write = fopen(to_write_path, "wb");
            ok_extracted = 1;
            opened_file_check(to_write);
        }

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
        for(unsigned int i = 0; i < filesize; i++) {
            char c = fgetc(archive);
            if (ok_extracted == 1) {
                fputc(c, to_write);
            }
        }

        // checks if end of archive (marked by a record of zeroes) is reached    
        if(ftell(archive) + RECORDSIZE == eof_pos) {
            break;
        }
    }

    if(ok_extracted == 1) {
        printf("> File extracted!\n");
    } else {
        printf("> File not found!\n");
    }

    fclose(to_write);
    fclose(archive);
}
