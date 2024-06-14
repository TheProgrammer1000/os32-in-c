#include <stdint.h>
#include "allocation.h"

struct Memory_Allocation memory_allocation;

void init_memory()
{
  memory_allocation.current_data_pool_address = (uint32_t *)0x01000000;
  memory_allocation.userSize = (uint32_t)0;
}

void malloc(int userSize)
{
  // int allocate_memory_temp[] = {};
  //  uint32_t *temp_data_pool_address = DATA_POOL_ADDRESS;
  if ((4096 % userSize) == 0 && userSize <= 4096) // This means there is only one entry
  {
    memory_allocation.current_data_pool_address = (uint32_t *)((uintptr_t)memory_allocation.current_data_pool_address + 0x1000); // First use the temp_data pointer and give it a pointer type here then we add the offset with 0x1000 then it will give this whole pointer the type (uint32 *)
    //*temp_data_pool_address += (1 * userSize); // 0x43 på 0x01000000 så först blocket är här
    memory_allocation.current_data_pool_address[0] = 0x41;

    //*temp_data_pool_address = 0x41; // Works we set the 0x01000000 to 0x41
    // Return bara en block på 4096 bytes
  }
  else
  {
    uint32_t blocks = (userSize + 4096 - 1) / 4096;

    /* For every block it will be a 0x1000 gap*/
    for (int i = 0; i < blocks; i++)
    {
      if (i == 0)
      { // If we are at the first block
        memory_allocation.current_data_pool_address = (uint32_t *)((uintptr_t)memory_allocation.current_data_pool_address + 0x1000);
        memory_allocation.current_data_pool_address[0] = 0xC1;
      }
      else if (i > 0 && i < (blocks - 1))
      {
        memory_allocation.current_data_pool_address = (uint32_t *)((uintptr_t)memory_allocation.current_data_pool_address + 0x1000);
        memory_allocation.current_data_pool_address[0] = 0x81;
      }

      if (i == (blocks - 1))
      { // This is the last block
        memory_allocation.current_data_pool_address = (uint32_t *)((uintptr_t)memory_allocation.current_data_pool_address + 0x1000);
        memory_allocation.current_data_pool_address[0] = 0x01;
      }
    }
    // 5000, 4096 > minst 2 bytes, ta resten av detta och använd
  }

  /*
   Hur vet vi hur många blocks av 4096 bytes vi ska allocera
  */
}

void free_memory(int userSize)
{
}
