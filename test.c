#include "cm108.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Very simple tool to test the cm108 ptt circuit.
 *
 * If it works, it should press the PTT button for
 * one second and then release it.
 */
int main(int argc, char** argv) {
    const char *path;
    path = cm108_find_device();
    
    if(!path) {
        printf("No device found. Sorry.\n");
        return 1;
    }

    printf("Found device %s\n", path);
    int fd = cm108_open(path);

    if(fd == -1) {
        printf("\nCould not open device! Have you installed the udev rules?\n");
        printf("You can retry as root.\n");
        exit(3);
    }
    
    printf("\nEnabling PTT...\n");
    cm108_ptt_set(fd, 1);
    sleep(1);
    printf("Disabling PTT...\n");
    cm108_ptt_set(fd, 0);
    printf("\nIf the RED led was on and is now off the PTT function works.\n");
}
