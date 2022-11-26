#include "IDT.h"
#include "IO.h"

#include "../../common/typedefs.h"
#include "../../common/keycodes.h"
#include "../drivers/display.h"

extern IDT64 _idt[256];
extern uint_64 isr1;
extern "C" void Load_IDT();

void(*MainKeyboardHandler)(uint_8 scanCode, uint_8 chr);
void(*MainCommandsHandler)(uint_8 scanCode, uint_8 chr);

void InitializeIDT() {

	_idt[1].zero = 0;
	_idt[1].offset_low = (uint_16)(((uint_64)&isr1 & 0x000000000000ffff));
	_idt[1].offset_mid = (uint_16)(((uint_64)&isr1 & 0x00000000ffff0000) >> 16);
	_idt[1].offset_high = (uint_32)(((uint_64)&isr1 & 0xffffffff00000000) >> 32);
	_idt[1].ist = 0;
	_idt[1].selector = 0x08;
	_idt[1].types_attr = 0x8e;

	remap_PIC();

	IO::outb(0x21, 0xfd);
	IO::outb(0xa1, 0xff);
	Load_IDT();
}

extern "C" void isr1_handler() {
    uint_8 scanCode = IO::inb(0x60);
    uint_8 chr = 0;

    if (scanCode < 0x3A) {
	    chr = KBSet::ScanCodeLookupTable[scanCode];
    }
    if (MainKeyboardHandler != 0) {
        MainKeyboardHandler(scanCode, chr);
    }
	if (MainCommandsHandler != 0) {
        MainCommandsHandler(scanCode, chr);
    }

	IO::outb(0x20, 0x20);
	IO::outb(0xa0, 0x20);
}
