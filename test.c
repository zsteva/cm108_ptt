#include "cm108.h"

/**
 * Very simple tool to test the cm108 ptt circuit.
 *
 * If it works, it should press the PTT button for
 * one second and then release it.
 */
int main(int argc, char** argv) {
    int fd = cm108_open();

    cm108_ptt_set(fd, 1);
    sleep(1);
    cm108_ptt_set(fd, 0);
}
