// Copyright 2019 Radu-Stefan Minea 314CA

#ifndef TEMA3_H
#define TEMA3_H

#include <stdio.h>

#define RECORDSIZE 512
#define LINE_MAX 512
#define OCTAL_BASE 8
#define CHKBLANKS "        "

union record {
    char charptr[RECORDSIZE];
    struct header {
        char name[100], mode[8], uid[8], gid[8], size[12], mtime[12], chksum[8],
            typeflag, linkname[100], magic[8], uname[32], gname[32],
            devmajor[8], devminor[8];
    } header;
};

void get_eof_pos(int *eof_pos, FILE *f);
// get_size
// check_end_archive
int to_decimal(int x, int base);
void opened_file_check(FILE *f);
void get_string(char *, int, int);
void get_mode(char *mode, char *p);
int get_chksum(union record filedata, int sum);
void list(char *archive_name);
void create(char *archive_name, char *directory_name);
void extract(char *file_name, char *archive_name);

#endif
