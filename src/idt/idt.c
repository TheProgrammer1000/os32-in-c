#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"
#include "io/io.h"

struct idt_desc idt_descriptors[DENINUXOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc *ptr); // Here we make a pointer to know where the idt is located
extern void int21h();
extern void no_interrupt();

void int21h_handler()
{
  char scancode = insb(0x60);

  if (scancode == 0x02) // Scancode for key '1'
  {
    print_text("Keyboard pressed!\n");
  }
  outb(0x20, 0x20);
}

void no_interrupt_handler()
{
  outb(0x20, 0x20);
}

void idt_zero()
{
  print_text("divide by zero error\n");
}

void idt_set(int interrupt_number, void *address)
{
  struct idt_desc *desc = &idt_descriptors[interrupt_number];
  desc->offset_1 = (uint32_t)address & 0x0000ffff;
  desc->selector = KERNEL_CODE_SELECTOR;
  desc->zero = 0x00;
  desc->type_attr = 0xEE;                   // We also set other bits in the dpl and so on. 40-43 Gate-type = 0b1110, 44 = 0, 45-46 dpl = 0b11, 47 p = 1. Result: 0b1110 1110
  desc->offset_2 = (uint32_t)address >> 16; // Higher part of the offset
}

void idt_init()
{
  memoryset(idt_descriptors, 0, sizeof(idt_descriptors));
  idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
  idtr_descriptor.base = (uint32_t)idt_descriptors;

  for (int i = 0; i < DENINUXOS_TOTAL_INTERRUPTS; i++)
  {
    idt_set(i, no_interrupt);
  }
  idt_set(0, idt_zero);
  idt_set(0x21, int21h); // Here we are calling the secound interrupt which is the keyboard press interrupt

  // Load the interrupt descriptor table
  idt_load(&idtr_descriptor);
}
