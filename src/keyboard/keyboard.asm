
global keyboard_setup
extern print_text

section .text
keyboard_setup:
    pusha
    mov al, 0xF1
    out 0x60, al

    cmp al, 0xFA
    je ACKReceived

ACKReceived:
    push ebx            ; Save the value of ebx (not needed for the call, but to preserve its value)
    mov ebx, successful ; Load address of the string into ebx
    push ebx            ; Push the address of the string onto the stack
    call print_text     ; Call the C function
    add esp, 4          ; Clean up the stack (remove the string address)
    pop ebx             ; Restore the original value of ebx

    popa
    ret

section .data
successful:
    db "Great, it is alright!", 0
