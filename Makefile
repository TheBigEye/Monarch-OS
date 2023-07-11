# ---------------------- Makefile for building Monarch OS ---------------------- #

# $@ = target file
# $< = first dependency
# $^ = all dependencies

# Source dir
SOURCE_DIR = source

BROWN =\033[0;33m
CYAN =\033[0;36m
GREEN =\033[0;32m
RESET =\033[0m

QEMU_ARGS := $(strip                       \
    -boot menu=on                          \
    -m 32M                                 \
    -cpu max                               \
    -device VGA,vgamem_mb=6                \
    -display sdl,gl=on                     \
    -audiodev dsound,id=0                  \
    -machine pcspk-audiodev=0              \
    -rtc base=localtime,clock=host         \
)

# C Source files
SOURCES = $(wildcard         		       \
	$(SOURCE_DIR)/common/*.c               \
	$(SOURCE_DIR)/kernel/CPU/CMOS/*.c      \
	$(SOURCE_DIR)/kernel/CPU/IDT/*.c       \
	$(SOURCE_DIR)/kernel/CPU/ISR/*.c       \
	$(SOURCE_DIR)/kernel/CPU/PIT/*.c       \
	$(SOURCE_DIR)/kernel/CPU/RTC/*.c       \
	$(SOURCE_DIR)/kernel/CPU/*.c           \
	$(SOURCE_DIR)/kernel/drivers/*.c       \
	$(SOURCE_DIR)/kernel/memory/*.c        \
	$(SOURCE_DIR)/kernel/power/*.c         \
	$(SOURCE_DIR)/kernel/*.c               \
)

# C headers files
HEADERS = $(wildcard                       \
	$(SOURCE_DIR)/common/*.h               \
	$(SOURCE_DIR)/kernel/CPU/CMOS/*.h      \
	$(SOURCE_DIR)/kernel/CPU/IDT/*.h       \
	$(SOURCE_DIR)/kernel/CPU/ISR/*.h       \
	$(SOURCE_DIR)/kernel/CPU/PIT/*.h       \
	$(SOURCE_DIR)/kernel/CPU/RTC/*.h       \
	$(SOURCE_DIR)/kernel/CPU/*.h           \
	$(SOURCE_DIR)/kernel/drivers/*.h       \
	$(SOURCE_DIR)/kernel/memory/*.h        \
	$(SOURCE_DIR)/kernel/power/*.h         \
	$(SOURCE_DIR)/kernel/*.h               \
)

# Object files
OBJ_FILES = ${SOURCES:.c=.o $(SOURCE_DIR)/boot/interrupt.o}

# Compiler and linker configuration
CC = i686-elf-gcc
LD = i686-elf-ld
CCFLAGS = -Os -Wall -Wextra -std=gnu99 -nostdlib -ffreestanding -ffunction-sections -fdata-sections -nostartfiles -nodefaultlibs
LDFLAGS = -m elf_i386 -Os

# First rule is the one executed when no parameters are fed to the Makefile
all: run

# Build the kernel binary
kernel.bin: $(SOURCE_DIR)/boot/kernel_main.o $(SOURCE_DIR)/boot/disk/binaries.o ${OBJ_FILES}
	@echo -e "${GREEN}[-]${RESET} Linking kernel binary '${BROWN}$@${RESET}' ..."
	@${LD} ${LDFLAGS} -o $@ -Ttext 0x1000 $^ --oformat binary


# Create the OS image (increase boot_zero seek if the kernel grows more)
#OS.img: $(SOURCE_DIR)/boot/boot_main.bin kernel.bin $(SOURCE_DIR)/boot/boot_zero.bin
#	@echo -e "${GREEN}[-]${RESET} Generating system floppy image '${BROWN}$@${RESET}' ..."
#	@dd bs=1024 count=1440 if=/dev/zero of=$@ > /dev/null 2>&1
#	@dd if=$(SOURCE_DIR)/boot/boot_main.bin of=$@ bs=512 count=1 conv=notrunc > /dev/null 2>&1
#	@dd if=kernel.bin of=$@ bs=512 seek=1 conv=notrunc > /dev/null 2>&1
#	@dd if=$(SOURCE_DIR)/boot/boot_zero.bin of=$@ bs=512 seek=85 conv=notrunc > /dev/null 2>&1


OS.img: $(SOURCE_DIR)/boot/boot_main.bin kernel.bin $(SOURCE_DIR)/boot/boot_zero.bin
	@echo -e "${GREEN}[-]${RESET} Generating system floppy image '${BROWN}$@${RESET}' ..."
	@cat $^ > $@


# Run the OS image in QEMU
run: OS.img
	@echo -e "${GREEN}[-]${RESET} Starting QEMU virtual machine for '${BROWN}$^${RESET}' ..."
	@qemu-system-i386 $< $(QEMU_ARGS)

# Display the contents of the OS image
echo: OS.img
	@xxd $<


# Compile C source files
%.o: %.c ${HEADERS}
	@echo -e "${CYAN}[i]${RESET} Compiling kernel component '${BROWN}$<${RESET}'"
	@${CC} ${CCFLAGS} -c $< -o $@

# Compile assembly source files
%.o: %.asm
	@echo -e "${CYAN}[i]${RESET} Assembling for bootloader '${BROWN}$<${RESET}'"
	@nasm $< -f elf -o $@

# Assemble binary files
%.bin: %.asm
	@echo -e "${CYAN}[i]${RESET} Generating binary file for '${BROWN}$<${RESET}'"
	@nasm $< -f bin -o $@


# Disassemble binary files
%.dis: %.bin
	@ndisasm -b 32 $< > $@

# Clean the project
clean:
	@echo -e "${GREEN}[-]${RESET} Cleaning object and output files ..."
	@$(RM) *.bin *.o *.dis *.elf *.img
	@find $(SOURCE_DIR) -name '*.o' -type f -delete
	@find $(SOURCE_DIR) -name '*.bin' -type f -delete
