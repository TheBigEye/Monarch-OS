#ifndef IO_H_
#define IO_H_

#include "common/typedefs.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

void io_wait();
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char value);
void outw(unsigned short port, unsigned short value);

unsigned char io_in_byte(unsigned short port);
void io_out_byte(unsigned short port, unsigned short value);
void io_out_word(unsigned short port, unsigned short value);

void RemapPic();

uint_32 get_ram_size();


#endif /* IO_H_ */
