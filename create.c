// Copyright 2019 Radu-Stefan Minea 314CA

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./tema3.h"


void create(char archive_name[], char directory_name[]) {
  char buffer[RECORDSIZE], padding[RECORDSIZE], *p, space[] = {" "},
                                                    colon[] = {":"};
  union record filedata;

  char files_path[RECORDSIZE], usermap_path[RECORDSIZE];

  strcpy(files_path, directory_name);
  strcpy(usermap_path, directory_name);

  strcat(files_path, "files.txt");
  strcat(usermap_path, "usermap.txt");

  FILE *files, *usermap, *archive;

  files = fopen(files_path, "r");
  // opened_file_check(files);

  archive = fopen(archive_name, "wb");
  // opened_file_check(archive);

  memset(padding, 0, sizeof(padding));

  while (fgets(buffer, sizeof(buffer), files)) {
    buffer[strlen(buffer) - 1] = '\0';
    // mode
    p = strtok(buffer, space);
    memset(filedata.header.mode, '0', sizeof(filedata.header.mode));
    get_mode(filedata.header.mode, p);
    // no_links
    p = strtok(NULL, space);
    // owner_name
    p = strtok(NULL, space);
    memset(filedata.header.uname, '\0', sizeof(filedata.header.uname));
    strcpy(filedata.header.uname, p);
    // owner_group
    p = strtok(NULL, space);
    memset(filedata.header.gname, '\0', sizeof(filedata.header.gname));
    strcpy(filedata.header.gname, p);
    // size
    memset(filedata.header.size, '0', sizeof(filedata.header.size));

    p = strtok(NULL, space);
    int size = atoi(p);
    memset(filedata.header.size, '0', sizeof(filedata.header.size));
    get_string(filedata.header.size, size, sizeof(filedata.header.size));
    // last_change_time
    memset(filedata.header.mtime, '\0', sizeof(filedata.header.mtime));

    p = strtok(NULL, space);
    struct tm last_modified_time = {0};
    strptime(p, "%Y-%m-%d", &last_modified_time);

    p = strtok(NULL, space);
    strptime(p, "%H:%M:%S", &last_modified_time);

    time_t rawtime = mktime(&last_modified_time);

    sprintf(filedata.header.mtime, "%lo", rawtime);

    // name/linkname
    memset(filedata.header.name, '\0', sizeof(filedata.header.name));
    memset(filedata.header.linkname, '\0', sizeof(filedata.header.linkname));

    p = strtok(NULL, space);
    p = strtok(NULL, space);
    strcpy(filedata.header.name, p);
    strcpy(filedata.header.linkname, p);

    // parsing usermap:
    usermap = fopen(usermap_path, "r");
    // opened_file_check(usermap);

    char ok = 0;
    int uid, gid;
    while (fgets(buffer, sizeof(buffer), usermap) && ok == 0) {
      p = strtok(buffer, ":");
      if (strcmp(p, filedata.header.uname) == 0) {
        ok = 1;
        // uid
        p = strtok(NULL, colon);
        p = strtok(NULL, colon);

        uid = atoi(p);
        memset(filedata.header.uid, '0', sizeof(filedata.header.uid));
        get_string(filedata.header.uid, uid, sizeof(filedata.header.uid));

        // gid
        p = strtok(NULL, colon);

        gid = atoi(p);
        memset(filedata.header.gid, '0', sizeof(filedata.header.gid));
        get_string(filedata.header.gid, gid, sizeof(filedata.header.gid));
      }
    }

    fclose(usermap);
    // typeflag
    filedata.header.typeflag = '\0';
    // magic
    strcpy(filedata.header.magic, "GNUtar ");
    // devmajor
    memset(filedata.header.devmajor, '\0', sizeof(filedata.header.devmajor));
    // devminor
    memset(filedata.header.devminor, '\0', sizeof(filedata.header.devminor));
    // chksum
    int sum = get_chksum(filedata, 0);
    memset(filedata.header.chksum, '0', sizeof(filedata.header.chksum));
    get_string(filedata.header.chksum, sum, sizeof(filedata.header.chksum));
    // writing HEADER to archive
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
    // writing CONTENT to archive
    char to_archive_path[RECORDSIZE];
    memset(to_archive_path, '\0', sizeof(to_archive_path));

    strcpy(to_archive_path, directory_name);
    strcat(to_archive_path, filedata.header.name);

    FILE *to_archive = fopen(to_archive_path, "rb");
    memset(buffer, '\0', sizeof(buffer));
    while (fread(buffer, 1, sizeof(buffer), to_archive)) {
      fwrite(buffer, 1, sizeof(buffer), archive);
      memset(buffer, '\0', sizeof(buffer));
    }
    fclose(to_archive);
  }
  fwrite(padding, 1, sizeof(padding), archive);

  fclose(files);
  fclose(archive);
  printf("< Done!\n");
}
