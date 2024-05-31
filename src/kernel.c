#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "allocation.h"

extern void problem();
// VGA_WIDTH 80
// VGA_HEIGHT 20

uint16_t *video_memory = 0;
uint8_t valueNow = 0;

int column_counter = 0;
int row_counter = 0;

// Keeping track on the absoulte address and adding the offset
// uint32_t *DATA_POOL_ADDRESS = (uint32_t *)0x01000000; // pointing to the memory address 0x01000000

uint32_t *DATA_POOL_ADDRESS = (uint32_t *)0x01000000;
uint32_t allocated_bytes_counter = 0;
uint32_t data_pool_size;
uint32_t *current_data_pool_address;

// Text and color
uint16_t terminal_make_char(char character, char color)
{
  return (color << 8) | character;
}

// make the screen clear
void terminal_putchar(int x, int y, char character, char colour)
{
  // Checking if column_counter == width then we need to add 1 to the row
  // also make column_conter to 0
  if (column_counter == VGA_WIDTH)
  {
    row_counter++;
    column_counter = 0;
  }
  video_memory[(y * VGA_WIDTH) + x] = terminal_make_char(character, colour);
}

void terminal_initialize()
{
  video_memory = (uint16_t *)(0xB8000);
  for (int y = 0; y < VGA_HEIGHT; y++)
  {
    for (int x = 0; x < VGA_WIDTH; x++)
    {
      terminal_putchar(x, y, ' ', 0);
    }
  }
}

// Get text length
size_t strlen(const char *str)
{
  size_t len = 0;
  while (str[len])
  {
    len++;
  }

  return len;
}

// Hur vet vi hur många gånger denna funktion anropas
void terminal_writechar(char character, char colour)
{
  terminal_putchar(column_counter, row_counter, character, colour);
  column_counter++; // Adding column_counter so the char will start at next column
}

void print_text(const char *str)
{
  size_t textLength = strlen(str);

  // This is checking if the text have a "\n" if it does then we break the line
  for (int i = 0; i < textLength; i++)
  {
    if (str[i] != 10)
    {
      terminal_writechar(str[i], 2);
    }
    else
    {
      for (int x = i; x < VGA_WIDTH; x++)
      {
        terminal_writechar(' ', 0);
      }
    }
  }
}

/*
  Function to allocate memory
  We want the malloc function to handle everything for us not to freeing but to select and allocte right memory for us
*/

void heap_init(uint32_t data_pool)
{
  data_pool_size = data_pool;
  current_data_pool_address = DATA_POOL_ADDRESS; // Initialize the current data pool address
}

void malloc(uint32_t userSize, uint32_t *DATA_POOL_ADDRESS)
{
  // uint32_t *temp_data_pool_address = DATA_POOL_ADDRESS;
  if ((4096 % userSize) == 0 && userSize <= 4096) // This means there is only one entry
  {
    current_data_pool_address = (uint32_t *)((uintptr_t)current_data_pool_address + 0x1000); // First use the temp_data pointer and give it a pointer type here then we add the offset with 0x1000 then it will give this whole pointer the type (uint32 *)
    //*temp_data_pool_address += (1 * userSize); // 0x43 på 0x01000000 så först blocket är här
    current_data_pool_address[0] = 0x41;

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
        current_data_pool_address = (uint32_t *)((uintptr_t)current_data_pool_address + 0x1000);
        current_data_pool_address[0] = 0xC1;
      }
      else if (i > 0 && i < (blocks - 1))
      {
        current_data_pool_address = (uint32_t *)((uintptr_t)current_data_pool_address + 0x1000);
        current_data_pool_address[0] = 0x81;
      }

      if (i == (blocks - 1))
      { // This is the last block
        current_data_pool_address = (uint32_t *)((uintptr_t)current_data_pool_address + 0x1000);
        current_data_pool_address[0] = 0x01;
      }
    }
    // 5000, 4096 > minst 2 bytes, ta resten av detta och använd
  }

  /*
   Hur vet vi hur många blocks av 4096 bytes vi ska allocera
  */
}

void kernel_main()
{
  // video_memory default 0xB000
  terminal_initialize();
  print_text("Hello World!\ntest");

  // Initialize the interrupt descriptor table
  idt_init();

  heap_init(100000000);
  // trying to use malloc
  malloc(4096, DATA_POOL_ADDRESS); // Here we pass in the DATA_POOL_ADDRESS we will keep in count and add offset with, then secound argument is how many bytes the user wants.
  malloc(5000, DATA_POOL_ADDRESS); // Here we pass in the DATA_POOL_ADDRESS we will keep in count and add offset with, then secound argument is how many bytes the user wants.
}
