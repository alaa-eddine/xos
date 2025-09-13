
;@Auteur : Alaa-eddine KADDOURI
;@Projet : X Operating System

;@Description :


[BITS 32]



%macro IRQA_HANDLER 1
extern idt_%1
global k_%1
k_%1:
align 4
cli
pushad
push ds
push es
mov ax, 0x10
mov ds, ax
mov es, ax
mov al, 0x20
out 0x20, al
call idt_%1
pop es
pop ds
popad
sti
iret
%endmacro

%macro IRQB_HANDLER 1
extern idt_%1
global k_%1
k_%1:
align 4
cli
pushad
push ds
push es
mov ax, 0x10
mov ds, ax
mov es, ax
mov al, 0x20
out 0x20, al
out 0xA0, al
call idt_%1
pop es
pop ds
popad
sti
iret
%endmacro



%macro INT_HANDLER 1
align 4
extern idt_%1
global k_%1
k_%1:
pushad
push ds
push es
mov ax, 0x10
mov ds, ax
mov es, ax
call idt_%1
pop es
pop ds
popad
iret
%endmacro




INT_HANDLER syscall

INT_HANDLER default_int

INT_HANDLER int0
INT_HANDLER int1
INT_HANDLER int2
INT_HANDLER int3
INT_HANDLER int4
INT_HANDLER int5
INT_HANDLER int6
INT_HANDLER int7
INT_HANDLER int8
INT_HANDLER int9
INT_HANDLER int10
INT_HANDLER int11
INT_HANDLER int12
INT_HANDLER int13
INT_HANDLER int14
INT_HANDLER int15
INT_HANDLER int16
INT_HANDLER int17
INT_HANDLER int18

IRQA_HANDLER irq0  
IRQA_HANDLER irq1  
IRQA_HANDLER irq2
IRQA_HANDLER irq3
IRQA_HANDLER irq4
IRQA_HANDLER irq5
IRQA_HANDLER irq6
IRQB_HANDLER irq7
IRQB_HANDLER irq8


