#include "IO.h"
#include "common/typedefs.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

void io_wait() {
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

// OLD (compatibility only) ---------------------------------------------------------------------------------------------------
unsigned char inb(unsigned short port){
	unsigned char value;
	asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));

	io_wait();
	return value;
}

void outb(unsigned short port, unsigned char value){
	asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
	io_wait();
}

void outw(unsigned short port, unsigned short value) {
	asm volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
	io_wait();
}


// Extended (new) -------------------------------------------------------------------------------------------------------------
unsigned char io_in_byte(unsigned short port) {
	return inb(port);
}

void io_out_byte(unsigned short port, unsigned short value) {
	outb(port, value);
}

void io_out_word(unsigned short port, unsigned short value) {
	outw(port, value);
}


void RemapPic(){
	uint_8 a1, a2;

	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 8);
	outb(PIC1_DATA, 4);
	outb(PIC2_DATA, 2);
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

uint_32 get_ram_size(){
	uint_32 eax, ebx, ecx, edx;
	eax = 0x80000005;
	asm volatile("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(eax));
	return (ecx >> 24);
}
