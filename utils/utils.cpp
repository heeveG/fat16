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

    std::cout << "Number of entries in root directory : " << bSector.sectorSize << std::endl;

    std::cout << "Number of reserved sectors : " << bSector.reservedSec << std::endl;

    uint16_t correctSign = 0xaa55;
    std::string isCorrect = bSector.actualSign == correctSign ? " - correct" : " - invalid";
    std::cout << "Signature is : " << std::hex << bSector.actualSign << std::dec << isCorrect << std::endl;

    return bSector;
}