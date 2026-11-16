section .bss
align 4096
page_directory:
	resb 4096		; reserve 4 KiB PD

align 4096
fpage_table:
	resb 4096		; reserve 4 KiB PT

section .text
global setup_paging
setup_paging:
	push edi
	cld
	xor eax, eax
	mov edi, page_directory
	mov ecx, 1024
	rep stosd
	mov edi, fpage_table
	mov eax, 0x00000003
	mov ecx, 1024

.fill_page_table:
	mov [edi], eax
	add eax, 0x1000
	add edi, 4
	loop .fill_page_table
	mov eax, fpage_table
	or eax, 0x00000003
	mov [page_directory], eax
	pop edi
	ret

global load_pd
load_pd:
	mov eax, page_directory
	mov cr3, eax
	ret

global enable_paging
enable_paging:
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	ret
