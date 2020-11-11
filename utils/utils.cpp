//
// Created by heeve on 11.11.20.
//

#include "utils.h"
#include "../fileEntry/fileEntry.h"

bootSector getImageChars(void *addr) {
    unsigned short sectorSize;
    readISO(addr, sectorSize, 11);
    std::cout << "Sector size : " << sectorSize << std::endl;

    unsigned char secPerCluster;
    readISO(addr, secPerCluster, 13);
    std::cout << "Sectors per cluster " << (int) secPerCluster << std::endl;

    unsigned char numFats;
    readISO(addr, numFats, 16);
    std::cout << "Number of FATS : " << (int) numFats << std::endl;

    unsigned short sizeFat;
    readISO(addr, sizeFat, 22);
    std::cout << "Size of FAT in sectors: " << sizeFat << "; in bytes: " << sizeFat * sectorSize << std::endl;

    unsigned short rootNumEntries;
    readISO(addr, rootNumEntries, 17);
    std::cout << "Number of entries in root directory : " << sectorSize << std::endl;

    unsigned short reservedSec;
    readISO(addr, reservedSec, 14);
    std::cout << "Number of reserved sectors : " << reservedSec << std::endl;

    unsigned short correctSign = 0xaa55, actualSign;
    readISO(addr, actualSign, 510);
    std::string isCorrect = actualSign == correctSign ? " - correct" : " - invalid";
    std::cout << "Signature is : " << std::hex << actualSign << std::dec << isCorrect << std::endl;

    return sectorSize + reservedSec * sectorSize + numFats * sizeFat * sectorSize;
}