#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <stddef.h>
#include <stdint.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00

#define DATA_POOL_MAX 0xC0000000

struct Memory_Allocation
{
  uint32_t userSize;
  uint32_t *current_data_pool_address;
  /* data */
};

// The user is going to request the amount of memory he wants, and then our job is the allocate it and see which is taken or not.
void init_memory();

void malloc(int);

void free_memory(int);
#endif
