[GLOBAL _read_cr0]
[GLOBAL _read_cr1]
[GLOBAL _read_cr2]
[GLOBAL _read_cr3]
[GLOBAL _read_cr4]

[GLOBAL _read_stack]
[GLOBAL _read_dr0]
[GLOBAL _read_dr1]
[GLOBAL _read_dr2]
[GLOBAL _read_dr3]
[GLOBAL _read_dr6]
[GLOBAL _read_dr7]
[GLOBAL _read_eax]
[GLOBAL _read_ebx]
[GLOBAL _set_dr7]



[BITS 32]

[SEGMENT .text]




_read_cr0:
	mov eax, cr0
	retn
_read_cr1:
	mov eax, cr1
	retn
_read_cr2:
	mov eax, cr2
	retn
_read_cr3:
	mov eax, cr3
	retn
_read_cr4:
	mov eax, cr4
	retn


_read_dr0:
	mov eax, dr0
	retn

_read_dr1:
	mov eax, dr1
	retn

_read_dr2:
	mov eax, dr2
	retn

_read_dr3:
	mov eax, dr3
	retn

_read_dr6:
	mov eax, dr6
	retn

_read_dr7:
	mov eax, dr7
	retn

_set_dr7:
	mov ax, 0xFF
	mov dr7, eax
	retn

_read_eax:
	retn

_read_ebx:
	mov eax, ebx
	retn


_read_stack:	
	push ebp	
	mov ebp, esp
	mov ebx, [ebp+8]	
	pop ebp

        mov  eax, [ebx] 	
	retn
