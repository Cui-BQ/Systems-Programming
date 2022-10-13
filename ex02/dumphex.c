#include <stdio.h>       // for printf
#include <inttypes.h>    // for uint8_t, PRIx8, etc.

int DumpHex(void *mem_addr, uint32_t num_bytes) {
    if (num_bytes <= 0) {
        printf("num_bytes must >= 1");
        return 1;
    }
    if (mem_addr <= 0) {
        printf("Not a valid address");
        return 2;
    }
    unsigned char* addr = mem_addr;
    printf("The %u bytes starting at %p are: ", num_bytes, mem_addr);
    for (int i = 0; i < num_bytes; i++) {
        printf("%02x ", *(addr+i));
    }
    printf("\n");
    return 0;
}