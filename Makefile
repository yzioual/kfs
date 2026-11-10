NAME     := kernel.bin
ISO      := kfs.iso

AS       := nasm
CC       := gcc
LD       := ld

ASFLAGS  := -f elf32
CFLAGS   := -m32 -ffreestanding -fno-builtin -fno-stack-protector \
            -nostdlib -nodefaultlibs -Wall -Wextra -Werror -Iinclude
LDFLAGS  := -m elf_i386 -T linker.ld -nostdlib

SRC_DIR  := src
OBJ_DIR  := build

ASM_SRC  := $(wildcard $(SRC_DIR)/*.asm)
C_SRC    := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(ASM_SRC:$(SRC_DIR)/%.asm=$(OBJ_DIR)/%.asm.o) \
            $(C_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.c.o)

all: $(NAME)

$(NAME): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(OBJ_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ISO): $(NAME)
	mkdir -p iso/boot/grub && cp $(NAME) iso/boot && cp grub.cfg iso/boot/grub/ \
		 && grub-mkrescue -o $(ISO) iso

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

check-header:
	grub-file --is-x86-multiboot kernel.bin && echo "multiboot OK"

clean:
	rm -rf $(OBJ_DIR) iso/

fclean: clean
	rm -rf $(NAME) $(ISO)

re: fclean all

.PHONY: all run clean fclean re
