//
// Created by heeve on 11.11.20.
//

#ifndef FAT16_UTILS_H
#define FAT16_UTILS_H

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include "../fileEntry/fileEntry.h"

template<typename T>
void readISO(void *addr, T &dest, int offset) {
    memcpy(&dest, (char *) addr + offset, sizeof(T));
}

bootSector getImageChars(void *addr, bootSector &bSector);

#endif //FAT16_UTILS_H
