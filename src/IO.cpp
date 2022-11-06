#include "IO.h"
#include "common/typedefs.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01



void IO::wait() {
	asm volatile("outb %%al, $0x80" : : "a"(0));
}

// OLD (compatibility only) ---------------------------------------------------------------------------------------------------
uint_8 IO::inb(uint_16 port){
	uint_8 value;
	asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));

	wait();
	return value;
}

void IO::outb(uint_16 port, uint_8 value){
	asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
	wait();
}

void IO::outw(uint_16 port, uint_16 value) {
	asm volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
	wait();
}


// Extended (new) -------------------------------------------------------------------------------------------------------------
uint_8 IO::in_byte(uint_16 port) {
	return inb(port);
}

void IO::out_byte(uint_16 port, uint_8 value) {
	outb(port, value);
}

void IO::out_word(uint_16 port, uint_16 value) {
	outw(port, value);
}


void RemapPic(){
	uint_8 a1, a2;

	a1 = IO::inb(PIC1_DATA);
	a2 = IO::inb(PIC2_DATA);

	IO::outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	IO::outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	IO::outb(PIC1_DATA, 0);
	IO::outb(PIC2_DATA, 8);
	IO::outb(PIC1_DATA, 4);
	IO::outb(PIC2_DATA, 2);
	IO::outb(PIC1_DATA, ICW4_8086);
	IO::outb(PIC2_DATA, ICW4_8086);

	IO::outb(PIC1_DATA, a1);
	IO::outb(PIC2_DATA, a2);
}

void sleep(uint_32 miliseconds) {
	uint_32 i;
	for (i = 0; i < miliseconds * 1000; i++) {
		asm volatile("outb %%al, $0x80" : : "a"(0));
	}
}



void cpuid(uint_32 code, uint_32* a, uint_32* b, uint_32* c, uint_32* d) {
    asm volatile("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "a"(code));
}

void beep(uint_32 frequency, uint_32 duration) {
	uint_32 divisor = 1193180 / frequency;
	IO::outb(0x43, 0xB6);
	IO::outb(0x42, (uint_8) (divisor & 0xFF));
	IO::outb(0x42, (uint_8) (divisor >> 8));
	IO::outb(0x61, IO::inb(0x61) | 3);
	sleep(duration);
	IO::outb(0x61, IO::inb(0x61) & 0xFC);
}
