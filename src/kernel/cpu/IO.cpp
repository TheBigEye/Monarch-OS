#include "IO.h"

#include "../../common/monarch.h"

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

void IO::write_8(uint_32 io_port, uint_8 val) {
	*((volatile uint_8 *)io_port) = val;
}

void IO::write_16(uint_32 io_port, uint_16 val) {
	*((volatile uint_16 *)io_port) = val;
}

void IO::write_32(uint_32 io_port, uint_32 val) {
	*((volatile uint_32 *)io_port) = val;
}

uint_32 IO::read(uint_32 io_port) {
	return *((volatile uint_32 *)io_port);
}

void remap_PIC(){
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
