[BITS 16]
[ORG 0x0]
jmp start


%include "../include/tools.inc"
%include "../include/macros.inc"
%include "../include/video.inc"
msg db 'Passage en mode protg...',0
;msg_shell_nf db 'ERREUR Le fichier shl32.com est introuvable',13,10
;             db 'Retours en mode 16 bits',13,10,0
prog db 'SHL32   COM',0

P_load:

         push es

         push ax   ; contient l'adresse ou on va charger le fichier



         mov ax, 19

lo1:
         push ax
         mov bx, fatbuffer

         Call  P_readsector
         mov cx, 16
         mov dl, 1
lo2:

         push cx


         push di   ; contient le nom du fichier  charger

         mov di, file
         mov bx, fatbuffer
         call P_buf2file


         mov bx, file
         call P_decompfile


         mov  si, filename
         pop  di
         push di


         mov  cx, 11
         repe cmpsb
         jz   Found

next1:
         inc  dl

         pop di
         pop cx
         loop lo2
         pop  ax
         cmp  ax, 32
         jge  NotFound
         inc  ax
         jmp  lo1

Found:
         pop di
         pop cx         ; cx et ax sont toujours dans la pile, on les depile
         pop ax

;         writeln found

         mov ax, 1
         call P_sl2p
         mov bx, fatbuffer
         mov ah, 02
         mov al, 9
         mov dl, 0
         int 13h

         pop ax
;         mov ax, 06000h
         mov es, ax

         mov bx, 0
         push bx


lo:
         mov ax, [cluster]


         pop bx
         add ax, 31
         call P_readsector
         add bx, 512
         push bx


         mov bx, fatbuffer
         mov ax, [cluster]
         call P_getnextcluster
         mov  [cluster], ax
         cmp ax, 0x0FFF
         jne lo

         pop bx   ;bx est toujours dans la pile on le depile

         mov ax, 1  ; fichier trouv
         jmp endi

;         jmp 09000h:0000h

NotFound:
              pop ax
              mov ax, 0  ; fichier non trouv
;              writeln notfound
;              jmp $
endi:
             pop es
             ret


start:
    mov ax, 06000h
    mov es, ax
    mov ds, ax


writeln msg

mov di, prog
mov ax, 0x0100
call P_load
cmp ax, 0
je exit
call P_cls
; initialisation du pointeur sur la GDT
	mov ax,gdtend	; calcule la limite de GDT
	mov bx,gdt
	sub ax,bx
	mov word [gdtptr],ax

	xor eax,eax		; calcule l'adresse lineaire de GDT
	xor ebx,ebx
	mov ax,ds
	mov ecx,eax
	shl ecx,4
	mov bx,gdt
	add ecx,ebx
	mov dword [gdtptr+2],ecx

; passage en modep
	cli
	lgdt [gdtptr]	; charge la gdt
	mov eax,cr0
	or ax,1
	mov cr0,eax		; PE mis a 1 (CR0)

	jmp next
next:
	mov ax,0x10		; segment de donne
	mov ds,ax
	mov fs,ax
	mov gs,ax
	mov es,ax
	mov ss,ax
	mov esp,0x9F000	

	jmp dword 0x8:0x1000    ; reinitialise le segment de code
exit:
;        write msg_shell_nf
        mov ah, 0   ;fonction 0 : retour au shell
        int 21h


;--------------------------------------------------------------------
gdt:
	db 0,0,0,0,0,0,0,0
gdt_cs:
	db 0xFF,0xFF,0x0,0x0,0x0,10011011b,11011111b,0x0
gdt_ds:
	db 0xFF,0xFF,0x0,0x0,0x0,10010011b,11011111b,0x0
gdtend:

;--------------------------------------------------------------------
gdtptr:
	dw	0	; limite
	dd	0	; base


;--------------------------------------------------------------------
[segment .bss]
file     resb 32    ;buffer Entree de FAT12
buffer   resb 512   ;buffer disquette
keybuf   resb 255   ;buffer clavier
fatbuffer resb 512*9;buffer FAT12
