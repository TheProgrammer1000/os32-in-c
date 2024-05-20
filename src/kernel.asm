[BITS 32]

global _start
global problem
extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable the A20 line
    in al, 0x92
    or al, 2
    out 0x92, al


    ; Remap the master PIC
    mov al, 00010001b ; This will get the PIC into a initialization mode
    out 0x20, al ; Tell master PIC

    mov al, 0x20 ; interrupt 0x20 is where master ISR should start
    out 0x21, al

    mov al, 00000001b       ; Put the PIC in x86 mode
    out 0x21, al

    ; End remap of the master PIC


    ; Enable the interrupts
    sti

    call kernel_main
    jmp $


problem:
    int 0

times 512-($ - $$) db 0