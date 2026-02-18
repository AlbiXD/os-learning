.code16
.global	_start
_start:
    movb %dl, bootdrive
    cli	
    # Zero out data segment registers
    xorw	%ax, %ax	    
    movw	%ax, %ds
    movw	%ax, %es
    movw	%ax, %ss
    movw 	$0x7c00, %sp


    movb	$0x0e, %ah
    movb	$'!', %al
    int	$0x10


#boot next 512 bytes

    movb bootdrive, %dl
    xorw %ax, %ax
    movw %ax, %es
    movb $0x02, %ah
    movb $0x01, %al
    movb $0x00, %ch
    movb $0x02, %cl
    movb $0x00, %dh
    movw $0x1000, %bx
    int $0x13
    
    jnc read_ok
    movb $0x0e, %ah
    movb $'E', %al
    int  $0x10
    jmp .
read_ok:
    movb $0x0e, %ah
    movb $'R', %al
    int  $0x10

    lgdt gdtdesc
    movl %cr0, %eax 
    orl  $0x1, %eax
    movl %eax, %cr0

    ljmp $0x08, $start32   

.code32
start32:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    xorw %ax, %ax
    movw %ax, %fs
    movw %ax, %gs

    movw $0x0f50, 0xb8000


    movl $0x90000, %esp
    mov $0x1000, %eax
    call *%eax
    jmp .
.align 8
gdt:
null_entry:
    .word 0, 0, 0, 0
code_entry:
    .word 0xFFFF	# limit low
    .word 0x0000	# base low
    .byte 0x00	# base mid
    .byte 0x9A	# access
    .byte 0xCf	# flags + limit
    .byte 0x00	# base high
data_entry:
    .word 0xFFFF
    .word 0x0000
    .byte 0x00
    .byte 0x92
    .byte 0xCF
    .byte 0x00
gdt_end:

gdtdesc:
    .word (gdt_end - gdt - 1)
    .long gdt
bootdrive: .byte 0
.org 510
.word 0xAA55
