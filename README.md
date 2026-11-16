# Kernel From Scratch (KFS)

A minimalistic 32-bit x86 monolithic kernel booted via GRUB and compliant with the Multiboot specification.

---

## Current Features

- **Boot & Multiboot**: Compliant with Multiboot 1 specifications; loaded at `1M` by GRUB with a 16 KiB kernel stack.
- **Segmentation (GDT)**: Custom Global Descriptor Table initialized at `0x800` defining kernel and user code, data, and stack segments.
- **Paging**: 2-level virtual memory paging identity-mapping the first 4 MiB (CR3 configured, CR0 paging bit enabled).
- **VGA Driver**: 80x25 text mode (`0xB8000`) with hardware cursor manipulation, scrolling, and color attribute support.
- **Virtual Screens**: 3 independent virtual consoles switchable dynamically via `Alt + F1`, `Alt + F2`, and `Alt + F3`.
- **PS/2 Keyboard**: Polling driver (scancode set 1) supporting US QWERTY layout, Shift, Caps Lock, backspace, and navigation keys.
- **Kernel `printk`**: Formatted printing supporting `%c`, `%s`, `%d`, `%i`, `%u`, `%x`, `%X`, `%p`, and `%%`.
- **Interactive Debug Shell**: Line editor with tab autocompletion and built-in commands:
  - `print-kernel-stack [words]`: Dumps stack limits, ESP/EBP registers, call frame backtrace, and memory hex dump.
  - `reboot`: Reboots machine via 8042 controller, Port 0x92, PCI reset, or triple fault.
  - `halt`: Halts the CPU execution (`cli` + `hlt`).
  - `echo <args>`: Prints arguments to the screen.
  - `clear`: Clears the active VGA console buffer.

---

## Project Structure

```text
.
├── grub.cfg          # GRUB boot menu configuration
├── include/          # Kernel header files
├── linker.ld         # Linker script (entry point at 1 MiB)
├── Makefile          # Build rules and QEMU runner
└── src/
    ├── boot.asm      # Multiboot header and entry point (_start)
    ├── gdt.c         # GDT table setup
    ├── gdt_flush.asm # Segment register reloading
    ├── kernel.c      # Kernel entry point (kmain)
    ├── keyboard.c    # PS/2 keyboard controller driver
    ├── paging.asm    # Page directory & page table configuration
    ├── printk.c      # Formatted string output implementation
    ├── screen.c      # Virtual console multiplexer
    ├── shell.c       # Interactive debug shell
    ├── string.c      # Freestanding string & memory utilities
    └── vga.c         # VGA text-mode driver
```

---

## Requirements

Ensure the following packages are installed on your Linux host:
- `gcc` (with 32-bit multilib support)
- `nasm`
- `binutils` (`ld`)
- `grub-pc-bin`, `grub-common`, `xorriso`
- `qemu-system-i386`

---

## Build & Run

- **Build kernel binary:**
  ```bash
  make
  ```

- **Verify Multiboot header:**
  ```bash
  make check-header
  ```

- **Generate bootable ISO:**
  ```bash
  make kfs.iso
  ```

- **Launch in QEMU:**
  ```bash
  make run
  ```

- **Clean build artifacts:**
  ```bash
  make clean    # removes build/ and iso/
  make fclean   # removes kernel.bin and kfs.iso
  ```
