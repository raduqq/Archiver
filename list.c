// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tema3.h"

void list(char *archive_name) {
  int filesize, eof_pos;
  union record filedata;
  char buffer[RECORDSIZE], name[sizeof(filedata.header.name)],
      filesize_aux[sizeof(filedata.header.size)];

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
    int filesize_position = sizeof(filedata.header.mode) +
                            sizeof(filedata.header.uid) +
                            sizeof(filedata.header.gid);

    fseek(archive, filesize_position, SEEK_CUR);
    fread(filesize_aux, sizeof(filesize_aux), 1, archive);

    sscanf(filesize_aux, "%d", &filesize);
    filesize = to_decimal(filesize, OCTAL_BASE);

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
