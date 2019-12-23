// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

void extract(char *file_name, char *archive_name) {
    unsigned int filesize;        
    union record filedata;
    char buffer[RECORDSIZE], name[sizeof(filedata.header.name)], filesize_aux[sizeof(filedata.header.size)]; 
    
    int ok_archive = 1, ok_file = 0;

    FILE *archive = fopen(archive_name, "rb");
    opened_file_check(archive);
    is_this_file_there(archive, &ok_archive);

    fseek(archive, 0, SEEK_END);
    int eof_pos = ftell(archive);
    fseek(archive, 0, SEEK_SET);

    if(ok_archive) {
        while(ftell(archive) < eof_pos && ok_file == 0) {
            // name
            int pos_init = ftell(archive);
            fread(name, sizeof(name), 1, archive);

            if(strcmp(name, file_name) == 0) {
                FILE *to_write = fopen(file_name, "wb");
                opened_file_check(to_write);
                ok_file = 1;
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
                    fwrite(buffer, sizeof(buffer), 1, to_write);
                }

                // checks if end of archive (marked by a record of zeroes) is reached    
                if(ftell(archive) + RECORDSIZE == eof_pos) {
                    break;
                }
            }
        }    
        if(ok_file == 1) {
            printf("> Done!\n");
        } else {
            printf("> File not found!\n");
        }
    }
}
