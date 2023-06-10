# Makefile for building Monarch OS

# $@ = target file
# $< = first dependency
# $^ = all dependencies

# Source dir
SOURCE_DIR = source

# C Source files
SOURCES = $(wildcard         		     \
	$(SOURCE_DIR)/common/*.c             \
	$(SOURCE_DIR)/kernel/CPU/*.c         \
	$(SOURCE_DIR)/kernel/drivers/*.c     \
	$(SOURCE_DIR)/kernel/memory/*.c      \
	$(SOURCE_DIR)/kernel/power/*.c       \
	$(SOURCE_DIR)/kernel/*.c             \
)

# C headers files
HEADERS = $(wildcard                     \
	$(SOURCE_DIR)/common/*.h             \
	$(SOURCE_DIR)/kernel/CPU/*.h         \
	$(SOURCE_DIR)/kernel/drivers/*.h     \
	$(SOURCE_DIR)/kernel/memory/*.h      \
	$(SOURCE_DIR)/kernel/power/*.h       \
	$(SOURCE_DIR)/kernel/*.h             \
)

# Object files
OBJ_FILES = ${SOURCES:.c=.o $(SOURCE_DIR)/boot/interrupt.o }

# Compiler and linker configuration
CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -m32 -g -std=gnu99 -ffreestanding -Os -Wall -Wextra -ffunction-sections -fdata-sections
LDFLAGS = -m elf_i386 -Os

# First rule is the one executed when no parameters are fed to the Makefile
all: run

# Build the kernel binary
kernel.bin: $(SOURCE_DIR)/boot/kernel_main.o $(SOURCE_DIR)/boot/disk/binaries.o ${OBJ_FILES}
	${LD} ${LDFLAGS} -o $@ -Ttext 0x1000 $^ --oformat binary

# Create the OS image
OS.img: $(SOURCE_DIR)/boot/boot_main.bin kernel.bin
	cat $^ > $@

# Run the OS image in QEMU
run: OS.img
	qemu-system-i386 $< -boot menu=on -m 32M -cpu max -vga std -display sdl,gl=on -audiodev dsound,id=0 -machine pcspk-audiodev=0 -rtc base=localtime,clock=host,driftfix=slew

# Display the contents of the OS image
echo: OS.img
	xxd $<

# Compile C source files
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

# Compile assembly source files
%.o: %.asm
	nasm $< -f elf -o $@

# Assemble binary files
%.bin: %.asm
	nasm $< -f bin -o $@

# Disassemble binary files
%.dis: %.bin
	ndisasm -b 32 $< > $@

# Clean the project
clean:
	$(RM) *.bin *.o *.dis *.elf *.img
	$(RM) $(SOURCE_DIR)/**/*.o $(SOURCE_DIR)/**/*.bin
	$(RM) $(SOURCE_DIR)/**/**/*.o $(SOURCE_DIR)/**/**/*.bin
