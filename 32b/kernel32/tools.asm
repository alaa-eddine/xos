
;@Auteur : Alaa-eddine KADDOURI
;@Projet : X Operating System

;@Description :






;****************************************************************************;
;*                              -=[ POCEDURES ]=-                           *;
;****************************************************************************;

[BITS 32]
global _reboot;
global jump_to_usertask;
global jump_to_kerntask;
global jump_to_tss;
global call_tss;
global v86;

jump_to_usertask:
	jmp 0x40:00
       retn

jump_to_kerntask:
	jmp 0x38:00
       retn

call_tss:
        push    ebp
        mov     ebp, esp
        call   dword far [ebp+4]
	mov     esp, ebp
        pop     ebp
        retn

jump_to_tss:
        push    ebp
        mov     ebp, esp
	cli
        jmp   dword far [ebp+4]
	sti
	mov     esp, ebp
        pop     ebp
        retn
	
	
;*****************************************;
;*      REDEMARRAGE DE L'ORDINATEUR      *;
;*****************************************;


; on essai de forcer un reboot via le controleur clavier   ;)
_reboot:
WKC:
    XOR         AL, AL
    IN          AL, 0x64
    TEST        AL, 0x02
    JNZ         WKC

    MOV         AL, 0xFC
    OUT         0x64, AL

[BITS 16]
v86:
	mov ax, 0xABCD
	int 3
	jmp $
	ret

	


