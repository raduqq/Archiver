// Copyright 2019 Radu-Stefan Minea 314CA

#ifndef TEMA3_H
#define TEMA3_H

#define RECORDSIZE 512
#define LINE_MAX 512

union record {
    char charptr[RECORDSIZE];
    struct header {
        char name[100], mode[8], uid[8], gid[8], size[12], mtime[12], chksum[8],
            typeflag, linkname[100], magic[8], uname[32], gname[32],
            devmajor[8], devminor[8];
    } header;
};

void create(char *, char *);
void list(char *);
void extract(char *, char *);

#endif