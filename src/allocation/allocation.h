#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <stddef.h>
#include <stdint.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN
#define HEAP_BLOCK_TABLE_ENTRY_FREE
#define DATA_POOL_BEGIN 0x01000000
#define DATA_POOL_MAX 0xC0000000

// The user is going to request the amount of memory he wants, and then our job is the allocate it and see which is taken or not.

void malloc(uint32_t *temp_allocate_address, uint32_t userSize);

void freeMemory();
#endif
