#ifndef IO_H_
#define IO_H_

#include "../../common/monarch.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

class IO {
    public:
        static void wait();
        static uint_8 inb(uint_16 port);
        static void outb(uint_16 port, uint_8 value);
        static void outw(uint_16 port, uint_16 value);

        static uint_8 in_byte(uint_16 port);
        static void out_byte(uint_16 port, uint_8 value);
        static void out_word(uint_16 port, uint_16 value);

        static void write_8(uint_32 io_port, uint_8 val);
        static void write_16(uint_32 io_port, uint_16 val);
        static void write_32(uint_32 io_port, uint_32 val);
        static uint_32 read(uint_32 io_port);
};

void sleep(uint_32 miliseconds);
void remap_PIC();

void beep(uint_32 frequency, uint_32 duration);

#endif /* IO_H_ */
