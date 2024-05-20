#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"

extern void problem();
// VGA_WIDTH 80
// VGA_HEIGHT 20

uint16_t *video_memory = 0;

uint8_t valueNow = 0;

int column_counter = 0;
int row_counter = 0;

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

void kernel_main()
{
  // video_memory default 0xB000
  terminal_initialize();
  print_text("Hello World!\ntest");

  // Initialize the interrupt descriptor table
  idt_init();
}