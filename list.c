// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tema3.h"

void list(char *archive_name) {
  int eof_pos;
  union record filedata;
  char name[sizeof(filedata.header.name)];
  FILE *archive;

  archive = fopen(archive_name, "rb");
  opened_file_check(archive);
  get_eof_pos(&eof_pos, archive);

    while (ftell(archive) < eof_pos) {
    // name
    int pos_init = ftell(archive);
    fread(name, sizeof(name), 1, archive);
    printf("> %s\n", name);

    // size
    int filesize = get_filesize(archive);

    // aducem cursorul acolo unde incepe efectiv continutul fisierului
    fseek(archive, RECORDSIZE + pos_init, SEEK_SET);

    // sarim peste continutul fisierului
    int record_blocks = filesize / RECORDSIZE;
    if (filesize % RECORDSIZE) {
      record_blocks++;
    }
    fseek(archive, record_blocks * RECORDSIZE, SEEK_CUR);

    // verificam daca urmeaza acel block de 0 = finalul arhivei
    if (ftell(archive) + RECORDSIZE == eof_pos) {
      break;
    }
  }
  fclose(archive);
}
