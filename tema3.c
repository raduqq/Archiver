// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tema3.h"

void opened_file_check(FILE *f) {
    if(f == NULL) {
        printf("< Failed!\n");
        exit(-1);
    }
}

void get_mode(char *mode, char *p) {
    unsigned int i, j;
    for (i = 4; i < 7; i++) {
      for (j = (i - 4) * 3 + 1; j <= (i - 3) * 3; j++) {
        if (j % 3 == 1 && p[j] == 'r') {
          mode[i] += 4;
        }
        if (j % 3 == 2 && p[j] == 'w') {
          mode[i] += 2;
        }
        if (j % 3 == 0 && p[j] == 'x') {
          mode[i] += 1;
        }
      }
    }
    mode[sizeof(mode) - 1] = '\0';
}

void get_string(char *field, int numerical_value) {
    unsigned int i, j;
    char value_in_string[sizeof(field)];

    sprintf(value_in_string, "%o", numerical_value);

    for (i = sizeof(field) - strlen(value_in_string) - 1, j = 0;
         i < sizeof(field) - 1 && j < strlen(value_in_string); i++, j++) {
      field[i] = value_in_string[j];
    }
    field[sizeof(field) - 1] = '\0';
}

int get_chksum(union record filedata, int sum) {
  unsigned int i;
  char chkblanks[] = {CHKBLANKS};
    for (i = 0; i < sizeof(chkblanks); i++) {
      sum += (int)chkblanks[i];
    }
    sum += (int)filedata.header.typeflag;
    for (i = 0; i < sizeof(filedata.header.name); i++) {
      sum += (int)filedata.header.name[i] + (int)filedata.header.linkname[i];
    }
    for (i = 0; i < sizeof(filedata.header.uname); i++) {
      sum += (int)filedata.header.uname[i] + (int)filedata.header.gname[i];
    }
    for (i = 0; i < sizeof(filedata.header.size); i++) {
      sum += (int)filedata.header.size[i] + (int)filedata.header.mtime[i];
    }
    for (i = 0; i < sizeof(filedata.header.mode); i++) {
      sum += (int)filedata.header.mode[i] + (int)filedata.header.uid[i] +
             filedata.header.gid[i];
      sum += (int)filedata.header.magic[i] + (int)filedata.header.devmajor[i] +
             (int)filedata.header.devminor[i];
    }
  return sum;
}