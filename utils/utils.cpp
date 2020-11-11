//
// Created by heeve on 11.11.20.
//

#include "utils.h"


bootSector getImageChars(void *addr, bootSector &bSector) {
    readISO(addr, bSector, 0);

    std::cout << "Sector size : " << bSector.sectorSize << std::endl;

    std::cout << "Sectors per cluster " << (int) bSector.secPerCluster << std::endl;

    std::cout << "Number of FATS : " << (int) bSector.numFats << std::endl;

    std::cout << "Size of FAT in sectors: " << bSector.sizeFat << "; in bytes: " << bSector.sizeFat * bSector.sectorSize
              << std::endl;

    std::cout << "Number of entries in root directory : " << bSector.rootNumEntries << "; in bytes: " << bSector.rootNumEntries * sizeof(fatDirEntry)
                                                                                                               << std::endl;

    std::cout << "Number of reserved sectors : " << bSector.reservedSec << std::endl;

    uint16_t correctSign = 0xaa55;
    std::string isCorrect = bSector.actualSign == correctSign ? " - correct" : " - invalid";
    std::cout << "Signature is : " << std::hex << bSector.actualSign << std::dec << isCorrect << std::endl;

    return bSector;
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

std::string readTime(uint16_t bitfield){
    return std::to_string((bitfield & 0x1F) * 2) + ":" + std::to_string((bitfield >> 5) & 0x3F) + ":" + std::to_string((bitfield >> 11) & 0x1F);
}

std::string readDate(uint16_t bitfield){
    return std::to_string(bitfield & 0x1F) + "-" + std::to_string((bitfield >> 5) & 0x0F) + "-" + std::to_string( ((bitfield >> 9) & 0x7F) + 1980);
}

void printFileEntry(struct fatDirEntry * de) {
    char filename[30];
    fatReadFilename(filename, de);

    std::cout << "Name: " << filename << std::endl;

    auto entrySize = fatIsDir(de) ? "0 (Directory)" : std::to_string(de->size);
    std::cout << "Size: " << entrySize << std::endl;

    std::cout << "Created: " << readDate(de->create_date) + " " + readTime(de->create_time) << std::endl;

    std::cout << "Last Modified: " << readDate(de->modify_date) + " " + readTime(de->modify_time) << std::endl;

    std::cout << "Attributes:" << std::endl;
    std::cout << "  Read-Only: " << (de->attributes & 0x01 ? "yes" : "no") << std::endl;
    std::cout << "  Hidden: " << (de->attributes & 0x02 ? "yes" : "no") << std::endl;
    std::cout << "  System: " << (de->attributes & 0x03 ? "yes" : "no") << std::endl;
    std::cout << "  Directory: " << (de->attributes & 0x10 ? "yes" : "no") << std::endl;
    std::cout << "  Archived: " << (de->attributes & 0x20 ? "yes" : "no") << std::endl << std::endl;

}

void getRootEntry(void *addr, uint16_t rootNumEntries, int rootOffset) {
    struct fatDirEntry entry{};

    for (int i = 0; i < rootNumEntries; ++i) {
        readISO(addr, entry, rootOffset + i * sizeof(fatDirEntry));

        if (!fatDirEntryExists(&entry)) continue;

        if (fatIsVolumeLabel(&entry)) continue;

        printFileEntry(&entry);
    }
}
