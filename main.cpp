#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {

    int fd = open("../hd0_just_FAT16_without_MBR.img", O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error while opening iso" << std::endl;
        exit(-1);
    }

    off_t sz = lseek(fd, 0, SEEK_END);
    if (sz == ((off_t) -1)) {
        std::cerr << "Error while seeking" << std::endl;
        exit(-1);
    }

    void *addr = mmap(NULL, sz, PROT_READ, MAP_PRIVATE, fd, 0);
    if (!addr) {
        std::cerr << "Error while mapping" << std::endl;
        exit(-1);
    }



    return 0;
}
