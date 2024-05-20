#ifndef IO_H
#define IO_H

unsigned char insb(unsigned short port);  // Input one byte to the port
unsigned short insw(unsigned short port); // Input one 2 bytes to the port

void outb(unsigned short port, unsigned char val); // Output one byte to the port
void outw(unsigned short port, unsigned char val); // Output 2 bytes to the port

#endif
