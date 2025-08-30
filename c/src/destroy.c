#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void physical_wipe(const char *device) {
    int fd = open(device, O_WRONLY);
    if (fd < 0) {
        perror("open device");
        return;
    }

    char pattern[4096];
    memset(pattern, 0xFF, sizeof(pattern));
    for (int i = 0; i < 10; i++) {
        lseek(fd, 0, SEEK_SET);
        write(fd, pattern, sizeof(pattern));
    }

    close(fd);
    printf("💥 Устройство %s перезаписано\n", device);
}