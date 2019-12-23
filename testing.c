// Copyright 2019 Radu-Stefan Minea 314CA

/// THIS SHOULD APPEAR ONLY IN BRANCH TESTING

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./tema3.h"

int main() {
  unsigned int i, j;
  char buffer[RECORDSIZE], padding[RECORDSIZE], *p, space[] = {" "},
                                                    colon[] = {":"};
  union record filedata;

  char directory_name[] = {"radutest/"}, archive_name[] = {"superarhiva.tar"};
  char files_path[RECORDSIZE], usermap_path[RECORDSIZE];

  strcpy(files_path, directory_name);
  strcpy(usermap_path, directory_name);

  strcat(files_path, "files.txt");
  strcat(usermap_path, "usermap.txt");

  FILE *files, *usermap, *archive;

  files = fopen(files_path, "r");
  opened_file_check(files);

  archive = fopen(archive_name, "wb");
  opened_file_check(archive);

  memset(padding, 0, sizeof(padding));

  while (fgets(buffer, sizeof(buffer), files)) {
    buffer[strlen(buffer) - 1] = '\0';
    // mode
    p = strtok(buffer, space);
    get_mode(filedata.header.mode, p);
    puts(filedata.header.mode);
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

    char size_aux[sizeof(filedata.header.size)];
    sprintf(size_aux, "%o", size);

    for (i = sizeof(filedata.header.size) - strlen(size_aux) - 1, j = 0;
         i < sizeof(filedata.header.size) - 1 && j < strlen(size_aux);
         i++, j++) {
      filedata.header.size[i] = size_aux[j];
    }
    filedata.header.size[sizeof(filedata.header.size) - 1] = '\0';

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
    memset(filedata.header.uid, '0', sizeof(filedata.header.uid));
    memset(filedata.header.gid, '0', sizeof(filedata.header.gid));

    int uid, gid;
    char uid_aux[sizeof(filedata.header.uid)],
        gid_aux[sizeof(filedata.header.gid)];

    usermap = fopen(usermap_path, "r");
    opened_file_check(usermap);

    char ok = 0;
    while (fgets(buffer, sizeof(buffer), usermap) && ok == 0) {
      p = strtok(buffer, ":");
      if (strcmp(p, filedata.header.uname) == 0) {
        ok = 1;
        // uid: MODULARIZE
        p = strtok(NULL, colon);
        p = strtok(NULL, colon);
        uid = atoi(p);
        sprintf(uid_aux, "%o", uid);

        for (i = sizeof(filedata.header.uid) - strlen(uid_aux) - 1, j = 0;
             i < sizeof(filedata.header.uid) - 1 && j < strlen(uid_aux);
             i++, j++) {
          filedata.header.uid[i] = uid_aux[j];
        }
        filedata.header.uid[sizeof(filedata.header.uid) - 1] = '\0';
        // gid: MODULARIZE
        p = strtok(NULL, colon);
        gid = atoi(p);
        sprintf(gid_aux, "%o", gid);
        for (i = sizeof(filedata.header.gid) - strlen(gid_aux) - 1, j = 0;
             i < sizeof(filedata.header.gid) - 1 && j < strlen(gid_aux);
             i++, j++) {
          filedata.header.gid[i] = gid_aux[j];
        }
        filedata.header.gid[sizeof(filedata.header.gid) - 1] = '\0';
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
    // chksum: MODULARIZE
    int sum = get_chksum(filedata, 0);

    memset(filedata.header.chksum, '0', sizeof(filedata.header.chksum));

    char sum_aux[sizeof(filedata.header.chksum)];
    sprintf(sum_aux, "%o", sum);

    for (i = sizeof(filedata.header.chksum) - strlen(sum_aux) - 1, j = 0;
         i < sizeof(filedata.header.chksum) - 1 && j < strlen(sum_aux);
         i++, j++) {
      filedata.header.chksum[i] = sum_aux[j];
    }
    filedata.header.chksum[sizeof(filedata.header.chksum) - 1] = '\0';

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
    while (fread(buffer, sizeof(buffer), 1, to_archive)) {
      fwrite(buffer, 1, sizeof(buffer), archive);
      memset(buffer, '\0', sizeof(buffer));
    }

    if (buffer[0] != '\0') {
      fwrite(buffer, 1, sizeof(buffer), archive);
    }

    fclose(to_archive);
  }
  fwrite(padding, 1, sizeof(padding), archive);

  fclose(files);
  fclose(archive);
  return 0;
}
