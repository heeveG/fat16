//
// Created by heeve on 11.11.20.
//

#include "utils.h"

int fatDirEntryExists(struct fatDirEntry * de);
int fatIsVolumeLabel(struct fatDirEntry * de);
int fatIsVolumeLabel(struct fatDirEntry * de);int fatIsFile(struct fatDirEntry * de);
int fatIsDir(struct fatDirEntry * de);
void fatReadFilename(char * output, struct fatDirEntry * de);

void printDirEntry(struct fatDirEntry * de) {
    char filename[13]; /* "FILENAME.EXT\0" */
//    struct fat_date date_created, date_modified, date_accessed;
//    struct fat_time time_created, time_modified;
    char * file_content;

    /* read filename */
    fatReadFilename(filename, de);

    /* read dates and times from bitfields */
//    fat_read_date(&date_created, de->create_date);
//    fat_read_date(&date_modified, de->modify_date);
//    fat_read_date(&date_accessed, de->access_date);
//    fat_read_time(&time_created, de->create_time);
//    fat_read_time(&time_modified, de->modify_time);

    printf("  %s\n", filename);
    printf("    bytes: %u  cluster: %u\n", de->size, de->start_cluster);
//    printf("    created:  %4u-%02u-%02u %02u:%02u:%02u\n",
//           date_created.year, date_created.month, date_created.day,
//           time_created.hour, time_created.minute, time_created.second);
//    printf("    modified: %4u-%02u-%02u %02u:%02u:%02u\n",
//           date_modified.year, date_modified.month, date_modified.day,
//           time_modified.hour, time_modified.minute, time_modified.second);
//    printf("    accessed: %4u-%02u-%02u\n",
//           date_accessed.year, date_accessed.month, date_accessed.day);
    printf("   ");
    printf(" ro:%s", de->attributes & 0x01 ? "yes" : "no");
    printf(" hide:%s", de->attributes & 0x02 ? "yes" : "no");
    printf(" sys:%s", de->attributes & 0x03 ? "yes" : "no");
    printf(" dir:%s", de->attributes & 0x10 ? "yes" : "no");
    printf(" arch:%s", de->attributes & 0x20 ? "yes" : "no");
    putchar('\n');

//    if (fat_is_file(de)) {
//        file_content = fat_read_file_from_dir_entry(fs, de);
//        printf("    Content:\n%s    <EOF>\n", file_content);
//        free(file_content);
//    }
}

bootSector getImageChars(void *addr, bootSector &bSector) {
    readISO(addr, bSector, 0);

    std::cout << "Sector size : " << bSector.sectorSize << std::endl;

    std::cout << "Sectors per cluster " << (int) bSector.secPerCluster << std::endl;

    std::cout << "Number of FATS : " << (int) bSector.numFats << std::endl;

    std::cout << "Size of FAT in sectors: " << bSector.sizeFat << "; in bytes: " << bSector.sizeFat * bSector.sectorSize
              << std::endl;

    std::cout << "Number of entries in root directory : " << bSector.rootNumEntries << std::endl << "; in bytes: " << bSector.rootNumEntries * bSector.sectorSize
                                                                                                               << std::endl;

    std::cout << "Number of reserved sectors : " << bSector.reservedSec << std::endl;

    uint16_t correctSign = 0xaa55;
    std::string isCorrect = bSector.actualSign == correctSign ? " - correct" : " - invalid";
    std::cout << "Signature is : " << std::hex << bSector.actualSign << std::dec << isCorrect << std::endl;

    return bSector;
}

void getRootEntry(void *addr, uint16_t rootNumEntries, int rootOffset) {
    struct fatDirEntry entry;

    printf("Root directory:\n");
    for (int i = 0; i < rootNumEntries; i++) {
        readISO(addr, entry, rootOffset);

        /* skip empty/unused entries */
        if (!fatDirEntryExists(&entry)) continue;

        /* skip volume label */
        if (fatIsVolumeLabel(&entry)) continue;

        printDirEntry(&entry);
    }
}

int fatDirEntryExists(struct fatDirEntry * de) {
    /* 0x00: null entry, 0xE5: marked as deleted */
    return !(*de->name == 0x00 || *de->name == 0xE5);
}

int fatIsVolumeLabel(struct fatDirEntry * de) {
    return (de->attributes & 0x08) == 0x08;
}

int fatIsFile(struct fatDirEntry * de) {
    /* not volume label, not directory */
    return (de->attributes & 0x18) == 0;
}

int fatIsDir(struct fatDirEntry * de) {
    /* not volume label, is directory */
    return (de->attributes & 0x18) == 0x10;
}

void fatReadFilename(char * output, struct fatDirEntry * de) {
    int len;
    char name[FAT_FILENAME_LENGTH + 1], extension[FAT_EXTENSION_LENGTH + 1];
    char * p;

    /* name, null-terminated, whitespace stripped */
    len = FAT_FILENAME_LENGTH;
    while (de->name[len - 1] == 0x20 && len > 0) len--;
    memmove(name, de->name, len);
    name[len] = 0;

    /* extension, null-terminated, whitespace stripped */
    len = FAT_EXTENSION_LENGTH;
    while (de->extension[len - 1] == 0x20 && len > 0) len--;
    memmove(extension, de->extension, len);
    extension[len] = 0;

    /* write name */
    p = stpcpy(output, name);

    /* write extension */
    if (*extension) {
        *p++ = '.';
        strcpy(p, extension);
    }
}
