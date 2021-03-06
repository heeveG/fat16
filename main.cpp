#include "utils/utils.h"

int main(int argc, char **argv) {
    if (argc > 2) {
        std::cerr << "Invalid number of arguments" << std::endl;
        exit(-1);
    }

    int fd = open(argc == 2 ? argv[1] : "../../hd0_just_FAT16_without_MBR.img", O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error while opening iso" << std::endl;
        exit(-1);
    }

    off_t sz = lseek(fd, 0, SEEK_END);
    if (sz == ((off_t) -1)) {
        std::cerr << "Error while seeking" << std::endl;
        exit(-1);
    }

    void *addr = mmap(nullptr, sz, PROT_READ, MAP_PRIVATE, fd, 0);
    if (!addr) {
        std::cerr << "Error while mapping" << std::endl;
        exit(-1);
    }

    bootSector bSector{};
    getImageChars(addr, bSector);
    int rootOffset = bSector.reservedSec * bSector.sectorSize +
                     bSector.numFats * bSector.sizeFat * bSector.sectorSize;

    std::cout << "Root offset: " << rootOffset << std::endl;

    getRootEntry(addr, bSector.rootNumEntries, rootOffset);

    return 0;
}
