#include "shell.h"
#include "types.h"
#include "vga.h"
#include "printk.h"
#include "keyboard.h"
#include "string.h"
#include "io.h"

#define MAX_LINE_LEN   128
#define MAX_ARGS       16

extern char stack_bottom[];
extern char stack_top[];

typedef struct s_command {
	const char	*name;
	const char	*desc;
	void		(*fn)(int argc, char *argv[]);
}	t_command;

static void	print_hex_byte(u8 val)
{
	static const char	hex[] = "0123456789abcdef";

	printk("%c%c", hex[(val >> 4) & 0xF], hex[val & 0xF]);
}

static void	print_hex32(u32 val)
{
	print_hex_byte((val >> 24) & 0xFF);
	print_hex_byte((val >> 16) & 0xFF);
	print_hex_byte((val >> 8) & 0xFF);
	print_hex_byte(val & 0xFF);
}

static void	cmd_print_kernel_stack(int argc, char *argv[])
{
	u32	s_bot;
	u32	s_top;
	u32	esp;
	u32	ebp;
	u32	stack_size;
	u32	stack_used;
	u32	stack_free;
	u32	cur_ebp;
	int	frame;
	u32	words_to_dump;
	u32	dump_addr;
	u32	dump_end;

	s_bot = (u32)stack_bottom;
	s_top = (u32)stack_top;
	__asm__ volatile ("mov %%esp, %0" : "=r"(esp));
	__asm__ volatile ("mov %%ebp, %0" : "=r"(ebp));

	stack_size = s_top - s_bot;
	stack_used = (esp <= s_top && esp >= s_bot) ? (s_top - esp) : 0;
	stack_free = (esp <= s_top && esp >= s_bot) ? (esp - s_bot) : 0;

	vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
	printk("================== KERNEL STACK INFO ==================\n");
	vga_set_color(VGA_WHITE, VGA_BLACK);
	printk("Stack Bottom : 0x"); print_hex32(s_bot); printk("\n");
	printk("Stack Top    : 0x"); print_hex32(s_top);
	printk(" (Size: %u bytes / %u KB)\n", stack_size, stack_size / 1024);
	printk("Current ESP  : 0x"); print_hex32(esp); printk("\n");
	printk("Current EBP  : 0x"); print_hex32(ebp); printk("\n");
	printk("Stack Used   : %u bytes\n", stack_used);
	printk("Stack Free   : %u bytes\n", stack_free);

	/* Call frame backtrace */
	vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
	printk("\n--- Call Frame Backtrace ---\n");
	vga_set_color(VGA_WHITE, VGA_BLACK);
	cur_ebp = ebp;
	frame = 0;
	while (cur_ebp != 0)
	{
		if (cur_ebp < s_bot || cur_ebp >= s_top - 8 || (cur_ebp & 3) != 0)
		{
			printk("  #%d [EBP 0x", frame); print_hex32(cur_ebp);
			printk("] (Outside valid stack boundaries)\n");
			break ;
		}
		u32 ret_addr = *((u32 *)(cur_ebp + 4));
		u32 next_ebp = *((u32 *)cur_ebp);
		printk("  #%d [EBP 0x", frame); print_hex32(cur_ebp);
		printk("] -> Return EIP: 0x"); print_hex32(ret_addr); printk("\n");
		frame++;
		if (next_ebp <= cur_ebp && next_ebp != 0)
		{
			printk("  (Corrupted / looped frame link)\n");
			break ;
		}
		cur_ebp = next_ebp;
	}
	if (frame == 0)
		printk("  (No frames available)\n");

	/* Raw stack memory dump */
	words_to_dump = 16;
	if (argc >= 2)
	{
		u32 parsed = 0;
		if (parse_u32(argv[1], &parsed) && parsed > 0)
			words_to_dump = (parsed > 64) ? 64 : parsed;
	}

	vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
	printk("\n--- Stack Memory Dump (from ESP: 0x");
	print_hex32(esp);
	printk(", %u words) ---\n", words_to_dump);
	vga_set_color(VGA_WHITE, VGA_BLACK);

	dump_addr = esp & ~0xF;
	dump_end = esp + words_to_dump * 4;
	if (dump_end > s_top)
		dump_end = s_top;

	while (dump_addr < dump_end)
	{
		print_hex32(dump_addr);
		printk(": ");
		for (int w = 0; w < 4; w++)
		{
			u32 addr = dump_addr + w * 4;
			if (addr < s_top)
			{
				print_hex32(*((u32 *)addr));
				printk(" ");
			}
			else
				printk("         ");
		}
		printk(" |");
		for (int b = 0; b < 16; b++)
		{
			u32 addr = dump_addr + b;
			if (addr < s_top)
			{
				u8 c = *((u8 *)addr);
				if (c >= 32 && c <= 126)
					printk("%c", c);
				else
					printk(".");
			}
			else
				printk(" ");
		}
		printk("|\n");
		dump_addr += 16;
	}
	vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
	printk("=======================================================\n");
	vga_set_color(VGA_WHITE, VGA_BLACK);
}

static void	cmd_reboot(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	u8	temp;
	u8	p92;
	struct s_null_idt {
		u16	limit;
		u32	base;
	} __attribute__((packed)) null_idt = {0, 0};

	printk("\nRebooting system...\n");
	__asm__ volatile ("cli");

	/* Method 1: 8042 Keyboard Controller pulse reset line */
	for (int i = 0; i < 10000; i++)
	{
		temp = inb(0x64);
		if (temp & 1)
			(void)inb(0x60);
		if (!(temp & 2))
			break ;
	}
	outb(0x64, 0xFE);
	for (volatile int i = 0; i < 500000; i++)
		;

	/* Method 2: Fast reset via Port 0x92 */
	p92 = inb(0x92);
	outb(0x92, p92 | 0x01);
	for (volatile int i = 0; i < 500000; i++)
		;

	/* Method 3: PCI reset (Port 0xCF9) */
	outb(0xCF9, 0x02);
	outb(0xCF9, 0x06);
	for (volatile int i = 0; i < 500000; i++)
		;

	/* Method 4: Triple fault */
	__asm__ volatile ("lidt %0; int $3" : : "m"(null_idt));

	for (;;)
		__asm__ volatile ("hlt");
}

static void	cmd_halt(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	printk("\nSystem halted. It is now safe to turn off your computer.\n");
	__asm__ volatile ("cli");
	for (;;)
		__asm__ volatile ("hlt");
}

static void	cmd_echo(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		printk("%s", argv[i]);
		if (i + 1 < argc)
			printk(" ");
	}
	printk("\n");
}

static void	cmd_clear(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	vga_clear();
}

static const t_command	commands[] = {
	{"print-kernel-stack", "Dump kernel stack bounds, backtrace, and memory", cmd_print_kernel_stack},
	{"reboot", "Reboot the machine", cmd_reboot},
	{"halt", "Halt the CPU", cmd_halt},
	{"echo", "Display input line or arguments", cmd_echo},
	{"clear", "Clear the terminal screen", cmd_clear}
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

static void	shell_tab_complete(char *buf, size_t *len_ptr, size_t max_len)
{
	size_t	len = *len_ptr;
	int		match_count = 0;
	int		last_match = -1;

	if (len == 0)
		return ;

	for (size_t i = 0; i < COMMAND_COUNT; i++)
	{
		if (ft_strncmp(commands[i].name, buf, len) == 0)
		{
			match_count++;
			last_match = (int)i;
		}
	}

	if (match_count == 1)
	{
		const char *suffix = commands[last_match].name + len;
		while (*suffix && len < max_len - 2)
		{
			buf[len++] = *suffix;
			vga_putchar(*suffix);
			suffix++;
		}
		if (len < max_len - 1)
		{
			buf[len++] = ' ';
			vga_putchar(' ');
		}
		buf[len] = '\0';
		*len_ptr = len;
	}
	else if (match_count > 1)
	{
		printk("\n");
		for (size_t i = 0; i < COMMAND_COUNT; i++)
		{
			if (ft_strncmp(commands[i].name, buf, len) == 0)
				printk("%s  ", commands[i].name);
		}
		printk("\n");
		vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
		printk("kfs> ");
		vga_set_color(VGA_WHITE, VGA_BLACK);
		for (size_t i = 0; i < len; i++)
			vga_putchar(buf[i]);
	}
}

static int	shell_readline(char *buf, size_t max_len)
{
	size_t	len;
	char	c;

	len = 0;
	buf[0] = '\0';
	while (1)
	{
		c = keyboard_getc();

		if (c == '\n')
		{
			vga_putchar('\n');
			buf[len] = '\0';
			return ((int)len);
		}
		if (c == '\b')
		{
			if (len > 0)
			{
				len--;
				buf[len] = '\0';
				vga_putchar('\b');
			}
			continue ;
		}
		if (c == 27) /* Escape: clear input line */
		{
			while (len > 0)
			{
				len--;
				vga_putchar('\b');
			}
			buf[0] = '\0';
			continue ;
		}
		if (c == '\t')
		{
			shell_tab_complete(buf, &len, max_len);
			continue ;
		}
		if (c >= 32 && c <= 126)
		{
			if (len < max_len - 1)
			{
				buf[len++] = c;
				buf[len] = '\0';
				vga_putchar(c);
			}
		}
	}
}

static int	shell_parse_args(char *line, char *argv[], int max_args)
{
	int	argc;

	argc = 0;
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (*line == '\0')
			break ;
		if (argc < max_args - 1)
			argv[argc++] = line;
		else
			break ;
		while (*line && !ft_isspace(*line))
			line++;
		if (*line)
		{
			*line = '\0';
			line++;
		}
	}
	argv[argc] = NULL;
	return (argc);
}

static void	shell_execute(int argc, char *argv[])
{
	if (argc == 0 || !argv[0] || argv[0][0] == '\0')
		return ;

	for (size_t i = 0; i < COMMAND_COUNT; i++)
	{
		if (ft_strcmp(argv[0], commands[i].name) == 0)
		{
			commands[i].fn(argc, argv);
			return ;
		}
	}

	/* Optional alias support */
	if (ft_strcmp(argv[0], "print-kernel-stack-thing") == 0)
	{
		cmd_print_kernel_stack(argc, argv);
		return ;
	}

	vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
	printk("kfs: command not found: '%s'. Available: print-kernel-stack, reboot, halt, echo, clear\n", argv[0]);
	vga_set_color(VGA_WHITE, VGA_BLACK);
}

static void	shell_welcome(void)
{
	vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
	printk("==================================================================\n");
	printk("              Welcome to KFS Minimal Debug Shell                  \n");
	printk("==================================================================\n");
	vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
	printk("Available commands:\n");
	printk("  - print-kernel-stack\n");
	printk("  - reboot\n");
	printk("  - halt\n");
	printk("  - echo <args>\n");
	printk("  - clear\n\n");
	vga_set_color(VGA_WHITE, VGA_BLACK);
}

void	shell_run(void)
{
	char	line[MAX_LINE_LEN];
	char	*argv[MAX_ARGS];
	int		argc;

	shell_welcome();

	while (1)
	{
		vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
		printk("kfs> ");
		vga_set_color(VGA_WHITE, VGA_BLACK);

		if (shell_readline(line, sizeof(line)) <= 0)
			continue ;

		argc = shell_parse_args(line, argv, MAX_ARGS);
		if (argc == 0)
			continue ;

		shell_execute(argc, argv);
	}
}
