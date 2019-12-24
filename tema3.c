// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tema3.h"

int to_decimal(int x, int base) {
  int decimal = 0, pow = 1;

  while (x) {
    decimal += (x % 10) * pow;
    pow *= base;
    x /= 10;
  }

  return decimal;
}

void get_eof_pos(int *eof_pos, FILE *f) {
  fseek(f, 0, SEEK_END);
  *eof_pos = ftell(f);
  fseek(f, 0, SEEK_SET);
}

void opened_file_check(FILE *f) {
  if (f == NULL) {
    printf("> Failed\n");
    exit(-1);
  }
}

void get_string(char *field, int numerical_value, int datasize) {
  #define DATASIZE_const datasize

  int i, j;
  char value_in_string[DATASIZE_const];

  sprintf(value_in_string, "%o", numerical_value);

  for (i = datasize - strlen(value_in_string) - 1, j = 0;
       i < datasize - 1 && j < (int)strlen(value_in_string); i++, j++) {
    field[i] = value_in_string[j];
  }
  field[datasize - 1] = '\0';
}

void get_mode(char *mode, char *p) {
  #define BEGIN_STRING_MODE 4
  #define END_STRING_MODE 7

  int i, j;
  for (i = BEGIN_STRING_MODE; i < END_STRING_MODE; i++) {
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

void write_header(union record filedata, char *padding, FILE *f) {
  fwrite(filedata.header.name, 1, sizeof(filedata.header.name), f);
  fwrite(filedata.header.mode, 1, sizeof(filedata.header.mode), f);
  fwrite(filedata.header.uid, 1, sizeof(filedata.header.uid), f);
  fwrite(filedata.header.gid, 1, sizeof(filedata.header.gid), f);
  fwrite(filedata.header.size, 1, sizeof(filedata.header.size), f);
  fwrite(filedata.header.mtime, 1, sizeof(filedata.header.mtime), f);
  fwrite(filedata.header.chksum, 1, sizeof(filedata.header.chksum), f);
  fwrite(&filedata.header.typeflag, 1, sizeof(filedata.header.typeflag),
           f);
  fwrite(filedata.header.linkname, 1, sizeof(filedata.header.linkname),
           f);
  fwrite(filedata.header.magic, 1, sizeof(filedata.header.magic), f);
  fwrite(filedata.header.uname, 1, sizeof(filedata.header.uname), f);
  fwrite(filedata.header.gname, 1, sizeof(filedata.header.gname), f);
  fwrite(filedata.header.devmajor, 1, sizeof(filedata.header.devmajor),
           f);
  fwrite(filedata.header.devminor, 1, sizeof(filedata.header.devminor),
           f);
  fwrite(padding, 1, RECORDSIZE - sizeof(filedata.header), f);
}
