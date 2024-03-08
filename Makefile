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
    -boot menu=off                         \
    -m 8M                                  \
    -cpu max                               \
    -device VGA,vgamem_mb=32               \
    -display sdl,gl=on                     \
    -audiodev dsound,id=0                  \
    -machine pcspk-audiodev=0              \
    -rtc base=localtime,clock=host         \
)

# C Source files
SOURCES = $(wildcard         		       \
	$(SOURCE_DIR)/common/*.c               \
	$(SOURCE_DIR)/kernel/CPU/BIOS/*.c      \
	$(SOURCE_DIR)/kernel/CPU/CMOS/*.c      \
	$(SOURCE_DIR)/kernel/CPU/FPU/*.c       \
	$(SOURCE_DIR)/kernel/CPU/IDT/*.c       \
	$(SOURCE_DIR)/kernel/CPU/ISR/*.c       \
	$(SOURCE_DIR)/kernel/CPU/PIT/*.c       \
	$(SOURCE_DIR)/kernel/CPU/RTC/*.c       \
	$(SOURCE_DIR)/kernel/CPU/*.c           \
	$(SOURCE_DIR)/kernel/drivers/*.c       \
	$(SOURCE_DIR)/kernel/memory/*.c        \
	$(SOURCE_DIR)/kernel/power/*.c         \
	$(SOURCE_DIR)/kernel/utils/*.c         \
	$(SOURCE_DIR)/kernel/*.c               \
)

# C headers files
HEADERS = $(wildcard                       \
	$(SOURCE_DIR)/common/*.h               \
	$(SOURCE_DIR)/kernel/CPU/BIOS/*.h      \
	$(SOURCE_DIR)/kernel/CPU/CMOS/*.h      \
	$(SOURCE_DIR)/kernel/CPU/FPU/*.H       \
	$(SOURCE_DIR)/kernel/CPU/IDT/*.h       \
	$(SOURCE_DIR)/kernel/CPU/ISR/*.h       \
	$(SOURCE_DIR)/kernel/CPU/PIT/*.h       \
	$(SOURCE_DIR)/kernel/CPU/RTC/*.h       \
	$(SOURCE_DIR)/kernel/CPU/*.h           \
	$(SOURCE_DIR)/kernel/drivers/*.h       \
	$(SOURCE_DIR)/kernel/memory/*.h        \
	$(SOURCE_DIR)/kernel/power/*.h         \
	$(SOURCE_DIR)/kernel/utils/*.h         \
	$(SOURCE_DIR)/kernel/*.h               \
)

# Object files
OBJECTS = ${SOURCES:.c=.o $(SOURCE_DIR)/boot/binaries.o $(SOURCE_DIR)/boot/bootmain.o}


# Compiler and linker configuration
CC = i686-elf-gcc
LD = i686-elf-ld
CCFLAGS = -std=gnu99 -m32 -Os -g0 -fno-pie -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin -ffreestanding
CWFLAGS = -Wall -Wextra -Werror
LDFLAGS = -m elf_i386 --allow-multiple-definition -nostdlib -s


### First rule is the one executed when no parameters are fed to the Makefile
all: kernel.elf run


# Build the kernel binary
kernel.elf: ${OBJECTS}
	@echo -e "${GREEN}[-]${RESET} Linking kernel binary '${BROWN}./$@${RESET}' ..."
	@${LD} -T $(SOURCE_DIR)/linker.ld ${LDFLAGS} ${OBJECTS} -o $@


# We maake the ISO image
OS.iso: kernel.elf
	@echo -e "${GREEN}[-]${RESET} Generating system ISO image at '${BROWN}./$@${RESET}' ..."
	@mkdir -p ./grub/temp/boot/grub
	@cp $< ./grub/temp/boot/kernel.elf
	@cp ./grub/menu.lst ./grub/temp/boot/grub/menu.lst
	@cp ./grub/grub ./grub/temp/boot/grub/grub
	@xorriso -as mkisofs -V Butterfly -R -b boot/grub/grub -no-emul-boot -quiet -boot-load-size 4 -boot-info-table -o $@ grub/temp/
	@$(RM) -rf ./grub/temp


# Run the OS image in QEMU
run: OS.iso
	@echo -e "${GREEN}[-]${RESET} Starting QEMU virtual machine for '${BROWN}./$^${RESET}' ..."
	@qemu-system-i386 -cdrom $< $(QEMU_ARGS)


# Display the contents of the OS image
echo: OS.iso
	@xxd $<


# Compile C source files
%.o: %.c ${HEADERS}
	@echo -e "${CYAN}[i]${RESET} Compiling component at '${BROWN}./$<${RESET}'"
	@${CC} ${CWFLAGS} ${CCFLAGS} -c $< -o $@


# Compile assembly source files
%.o: %.asm
	@echo -e "${CYAN}[i]${RESET} Assembling component at '${BROWN}./$<${RESET}'"
	@nasm $< -Ox -f elf -o $@


# Assemble binary files
%.bin: %.asm
	@echo -e "${CYAN}[i]${RESET} Generating binary file for '${BROWN}./$<${RESET}'"
	@nasm $< -f bin -o $@


# Disassemble binary files
%.dis: %.bin
	@ndisasm -b 32 $< > $@


# Clean the project folder
clean:
	@echo -e "${GREEN}[-]${RESET} Cleaning objects and output files ..."
	@$(RM) *.bin *.o *.dis *.elf *.img *.iso
	@$(RM) -rf ./grub/temp
	@find $(SOURCE_DIR) -name '*.o' -type f -delete
	@find $(SOURCE_DIR) -name '*.bin' -type f -delete
