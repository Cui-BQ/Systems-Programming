#include<stdio.h> 
#include<string.h>

#include "my_memmove.h"
void* my_memmove ( void * destination, const void * source, size_t num ){
    unsigned char* dest = destination;
    const unsigned char* src = source;
    for (size_t i = 0; i < num; i++){
       dest[i] = src[i]; 
    }
    return destination;
}