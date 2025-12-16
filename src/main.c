#include "panik.h"
#include <stdio.h>

int main(int argc, char** argv) {
    printf("Hello, World!\n");
    printf("I got %d arguments:\n", argc);
    for (int i = 0; i < argc; i++) {
        printf(" - %s\n", argv[i]);
    }
    return 0;
}
