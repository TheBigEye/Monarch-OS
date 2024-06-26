#include "VGA.h"

#include "../../CPU/PIT/timer.h"
#include "../../CPU/HAL.h"
#include "../../memory/memory.h"

static uint32_t getSegment(void) {
	uint32_t segment;

	writeByteToPort(GRAPHICS_INDEX, 6);
	segment = readByteFromPort(GRAPHICS_DATA);

	segment >>= 2;
	segment &= 3;

	switch (segment) {
        case 0:
        case 1: segment = 0xA0000; break;
        case 2: segment = 0xB0000; break;
        case 3: segment = 0xB8000; break;
	}
	return segment;
}

static inline void writeRegister(uint16_t port, uint8_t reg, uint8_t val) {
    writeByteToPort(port, reg); /* Select register */
    writeByteToPort(port + 1, val); /* Do writing through data port */
}

static inline uint8_t readRegister(uint16_t port, uint8_t reg) {
    writeByteToPort(port, reg); /* Select register */
    return readByteFromPort(port + 1); /* Do reading through data port */
}

static inline void writeVideoMemory(uint32_t destOffset, uint8_t *source, uint32_t count) {
    memoryCopy((void *)(getSegment() + destOffset), (void *) source, count);
}

static void setPlane(uint8_t plane) {
	uint8_t mask;

	plane &= 3;
	mask = 1 << plane;

    /* set read plane */
	writeByteToPort(GRAPHICS_INDEX, REG_GRAPHICS_MAP_READ);
	writeByteToPort(GRAPHICS_DATA, plane);

    /* set write plane */
	writeByteToPort(SEQUENCER_INDEX, REG_SEQUENCER_MASK);
	writeByteToPort(SEQUENCER_DATA, mask);
}

static void writeColorPallete(uint8_t index, uint32_t RGB) {
    // Write the color index to the digital/analog controller port
    writeByteToPort(DIGANALOG_WRITE_INDEX, index);

    // For each color component (R, G, B)
    for (uint16_t i = 0; i < 3; i++) {
        // Shift the color bits and take only the last 6 bits
        uint8_t colorComponent = (RGB >> (i * 6)) & 0x3f;

        // Write the color component to port 0x3c9
        writeByteToPort(DIGANALOG_DATA, colorComponent);
    }
}


/*
static void vpokeb(unsigned off, char val) {
   char *pokebyte=(char *)(getSegment()+off);
   *pokebyte=val;
}

static char vpeekb(unsigned off) {
   char *pokebyte=(char *)(getSegment()+off);
	return (*pokebyte);
}
*/

static void writeFont(uint8_t *buffer, uint8_t fontHeight) {
    uint8_t seq2, seq4, gc4, gc5, gc6;

    /* save registers
    setPlane() modifies GC 4 and SEQ 2, so save them as well */
    writeByteToPort(SEQUENCER_INDEX, 2);
    seq2 = readByteFromPort(SEQUENCER_DATA);

    writeByteToPort(SEQUENCER_INDEX, 4);
    seq4 = readByteFromPort(SEQUENCER_DATA);

    /* turn off even-odd addressing (set flat addressing)
    assume: chain-4 addressing already off */
    writeByteToPort(SEQUENCER_DATA, seq4 | 0x04);

    writeByteToPort(GRAPHICS_INDEX, 4);
    gc4 = readByteFromPort(GRAPHICS_DATA);

    writeByteToPort(GRAPHICS_INDEX, 5);
    gc5 = readByteFromPort(GRAPHICS_DATA);

    /* turn off even-odd addressing */
    writeByteToPort(GRAPHICS_DATA, gc5 & ~0x10);

    writeByteToPort(GRAPHICS_INDEX, 6);
    gc6 = readByteFromPort(GRAPHICS_DATA);
    /* turn off even-odd addressing */
    writeByteToPort(GRAPHICS_DATA, gc6 & ~0x02);

    /* write font to plane P4 */
    setPlane(2);

    /* write font 0 */
    for (uint16_t i = 0; i < 256; i++) {
        // We overwrite the video font bitmap in memory, this is DANGEROUS!
        writeVideoMemory(16384U * 0 + i * 32, buffer, fontHeight);
        buffer += fontHeight;
    }

    /* restore registers */
    writeByteToPort(SEQUENCER_INDEX, 2);
    writeByteToPort(SEQUENCER_DATA, seq2);
    writeByteToPort(SEQUENCER_INDEX, 4);
    writeByteToPort(SEQUENCER_DATA, seq4);
    writeByteToPort(GRAPHICS_INDEX, 4);
    writeByteToPort(GRAPHICS_DATA, gc4);
    writeByteToPort(GRAPHICS_INDEX, 5);
    writeByteToPort(GRAPHICS_DATA, gc5);
    writeByteToPort(GRAPHICS_INDEX, 6);
    writeByteToPort(GRAPHICS_DATA, gc6);
}


void configureScreen(uint8_t *registers, bool isVideoMode) {
    /* We need clear the text mode buffer 0xB8000 always! */
    fastWideMemorySet((uint16_t *) TEXTMODE_BUFFER, (0 << 8) | (0x00), TEXTMODE_SIZE);

    /* write MISCELLANEOUS register */
    writeByteToPort(MISCELLANEOUS_WRITE, *registers);
    registers++;

    /* write SEQUENCER registers */
    for (uint8_t i = 0; i < NUM_SEQUENCER_REGS; i++) {
        writeByteToPort(SEQUENCER_INDEX, i);
        writeByteToPort(SEQUENCER_DATA, *registers);
        registers++;
    }

    /* unlock CRTC registers */
    writeByteToPort(CATHODERAY_INDEX, 0x03);
    writeByteToPort(CATHODERAY_DATA, readByteFromPort(CATHODERAY_DATA) | 0x80);
    writeByteToPort(CATHODERAY_INDEX, 0x11);
    writeByteToPort(CATHODERAY_DATA, readByteFromPort(CATHODERAY_DATA) & ~0x80);

    /* make sure they remain unlocked */
    registers[0x03] |= 0x80;
    registers[0x11] &= ~0x80;

    /* write CRTC registers */
    for (uint8_t i = 0; i < NUM_CATHODERAY_REGS; i++) {
        writeByteToPort(CATHODERAY_INDEX, i);
        writeByteToPort(CATHODERAY_DATA, *registers);
        registers++;
    }

    /* write GRAPHICS CONTROLLER registers */
    for (uint8_t i = 0; i < NUM_GRAPHICS_REGS; i++) {
        writeByteToPort(GRAPHICS_INDEX, i);
        writeByteToPort(GRAPHICS_DATA, *registers);
        registers++;
    }

    /* write ATTRIBUTE CONTROLLER registers */
    for (uint8_t i = 0; i < NUM_ATTRIBUTE_REGS; i++) {
        (void) readByteFromPort(INPUT_STATE_READ);
        writeByteToPort(ATTRIBUTE_INDEX, i);
        writeByteToPort(ATTRIBUTE_WRITE, *registers);
        registers++;
    }

    /*if (!isVideoMode) {
        for (uint8_t i = 0; i < 16; i++) {
            writeColorPallete(i, color_palette[i]);
        }
    }*/

    /* lock 16-color palette and unblank display */
    (void) readByteFromPort(INPUT_STATE_READ);
    writeByteToPort(ATTRIBUTE_INDEX, 0x20);

    if (!isVideoMode) {
        writeFont(small_font, 8);
    }
}
