section .stivalehdr

stivale_header:
	dq stack.top
	dw 0
	dw 0
	dw 0
	dw 0
	dw 0
	dw 0
	dw 0
	dw 0
	dq

section .bss

stack:
    resb 4096
  .top:
	
section .data
global GDT64
GDT64:
	.Null: equ $ - GDT64
	dq 0
	.Code: equ $ - GDT64
	dw 0
	dw 0 
	db 0
	db 10011010b
	db 00100000b
	db 0
	.Data: equ $ - GDT64
	dw 0
	dw 0
	db 0
	db 10010010b
	db 00000000b
	db 0
GDT_END:

GDT_PTR:
	dw GDT_END - GDT64 - 1
	dq GDT64



section .text

extern kmain
global exec_start ; So qloader2 can find the entry point properly
exec_start:
	lgdt [GDT_PTR]

	mov rax, rsp
	push 0x10
	push rax
	pushf
	push 0x8
	push run_kernel
	iretq

run_kernel:
	mov ax, 0x10
	
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov gs, ax
	mov fs, ax

	call kmain

	cli
	hlt

	call kmain

    ; Halt if the kernel exits for some reason
    cli
    hlt
