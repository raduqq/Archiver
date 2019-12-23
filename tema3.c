// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

void opened_file_check(FILE *f) {
    if(f == NULL) {
        printf("< Failed!\n");
        exit(-1);
    }
}
void get_owner_name(union record filedata, char *p) {
    char space[] = {" "};
    memset(filedata.header.uname, '\0', sizeof(filedata.header.uname));
    p = strtok(NULL, space);
    strcpy(filedata.header.uname, p);
}
void get_owner_group(union record filedata, char *p) {
    char space[] = {" "};
    memset(filedata.header.gname, '\0', sizeof(filedata.header.gname));
    p = strtok(NULL, space);
    strcpy(filedata.header.gname, p);
}
void get_mode(union record filedata, char *buffer, char *p) {
    unsigned int i, j;
    char space[] = {" "};    
    memset(filedata.header.mode, '0', sizeof(filedata.header.mode));
    p = strtok(buffer, space);
    for (i = 4; i < 7; i++) {
      for (j = (i - 4) * 3 + 1; j <= (i - 3) * 3; j++) {
        if (j % 3 == 1 && p[j] == 'r') {
          filedata.header.mode[i] += 4;
        }
        if (j % 3 == 2 && p[j] == 'w') {
          filedata.header.mode[i] += 2;
        }
        if (j % 3 == 0 && p[j] == 'x') {
          filedata.header.mode[i] += 1;
        }
      }
    }
    filedata.header.mode[sizeof(filedata.header.mode) - 1] = '\0';
}
void write_header(union record filedata, char *padding, FILE *archive) {
    fwrite(filedata.header.name, 1, sizeof(filedata.header.name), archive);
    fwrite(filedata.header.mode, 1, sizeof(filedata.header.mode), archive);
    fwrite(filedata.header.uid, 1, sizeof(filedata.header.uid), archive);
    fwrite(filedata.header.gid, 1, sizeof(filedata.header.gid), archive);
    fwrite(filedata.header.size, 1, sizeof(filedata.header.size), archive);
    fwrite(filedata.header.mtime, 1, sizeof(filedata.header.mtime), archive);
    fwrite(filedata.header.chksum, 1, sizeof(filedata.header.chksum), archive);
    fwrite(&filedata.header.typeflag, 1, sizeof(filedata.header.typeflag),
           archive);
    fwrite(filedata.header.linkname, 1, sizeof(filedata.header.linkname),
           archive);
    fwrite(filedata.header.magic, 1, sizeof(filedata.header.magic), archive);
    fwrite(filedata.header.uname, 1, sizeof(filedata.header.uname), archive);
    fwrite(filedata.header.gname, 1, sizeof(filedata.header.gname), archive);
    fwrite(filedata.header.devmajor, 1, sizeof(filedata.header.devmajor),
           archive);
    fwrite(filedata.header.devminor, 1, sizeof(filedata.header.devminor),
           archive);
    fwrite(padding, 1, sizeof(padding) - sizeof(filedata.header), archive);
}