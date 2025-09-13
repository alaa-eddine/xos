

;@Auteur : Alaa-eddine KADDOURI
;@Projet : X Operating System

;@Description :



[BITS 16]
%include "macros.inc"   ;**
%include "colors.inc"   ;**

%define seg_palette 0x8000   
%define VBEINFO 0x8F00

extern kernelbase
extern main

;global _start

_start:

cli
mov    AX,9000h
mov    SS,AX
mov    SP,0h
sti



mov ax, cs
mov es, ax
mov ds, ax


;Vérification de la présence d'un processeur 386
call C386

;Activation de la ligne A20
call EnableA20


%ifdef DO_SPLASH
;Affichage du logo X-OS
call P_draw_logo
%endif

__OK:


mov ah, 0
mov al, 3h
int 10h
;initialisation du mode texte
call      P_inittext
mov       bh, byte 31		;texte blanc sur fond bleu
call      P_cls	 	 
mov       bl, 4			;cadre rouge
call      P_setbkcolor


;lecture des informations vesa
push es
mov ax, VBEINFO
mov es, ax
mov ax, 4f00h
mov di, 0
int 10h

pop es



;initialisation du pointeur sur la GDT
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

jmp next		;ajustement des registres
next:
mov ax,0x10		; segment de donne
mov ds,ax
mov fs,ax
mov gs,ax
mov es,ax
mov ss,ax
mov esp,0x9F000 





mov ebx, 0x90FF2   ;recuperation de la taille du kernel
mov ecx, [ebx]     ;qui a été sauvée à cette addresse par le secteur de boot
shr ecx, 2	;on divise la taille par 4 car on va charger 
;le kernel par doubles mots

mov esi, 0x1000	    ;le kernel se trouve actuellement ici
mov ebx, 0x100000   ;on veut le déplacer ici

.lo
lodsd			
mov [es:ebx], eax
add ebx, 4
loop .lo


jmp dword 0x8:0x100000    ; reinitialise le segment de code



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






;----------------------------------------------
; Procédures pour l'activation de la porte A20
;----------------------------------------------
EnableA20:
push ax
mov ah,0dfh
call GateA20
or al,al
jz EnableA20_ok
stc
EnableA20_ok:
pop ax
ret
GateA20:
pushf
cli
call Empty8042
jnz GateA20_fail
out 0edh, ax
mov al,0D1h
out 64h,al
call Empty8042
jnz GateA20_fail
mov al,ah
out 60h,al
call Empty8042
push cx
mov cx,14h
GateA20_loop:
out 0edh,ax
loop GateA20_loop
pop cx
GateA20_fail:
popf
ret
Empty8042:
push cx
xor cx,cx
Empty8042_try:
out 0edh,ax
in al,64h
and al,2
loopnz Empty8042_try
pop cx
ret





C386:
push si
pushf

xor ah, ah
push ax
popf

pushf
pop ax
and ah, 0x0f
cmp ah, 0x0f
je .No386

mov ah, 0x0f
push ax
popf

pushf
pop ax
and ah, 0x0f
jz .No386
popf
jmp .386

.No386:
mov si, msgno386
call P_print
jmp $

.386        

pop si	
ret             ; tout va bien on se casse



;--------------------------------------------------------------------
P_draw_logo:

mov ax, 01201h      ;on interdit la construction de l'écran pendant l'affichage
mov bl, 036h
int 10h

mov di, logoname
mov ax, 08000h
call P_load 
cmp ax, 0
je .cont
jmp .end
.end


mov ah, 0
mov al, 13h
int 10h

push ds
mov ax, 08000h
mov ds, ax
;mov cx, 0
call      P_aff_grlogo
pop ds	


mov ax, 01200h      ;Le logo à été chargé :: on réautorise la construction de l'écran
mov bl, 036h
int 10h


;une petite boucle pour attendre 2 seconds avant d'affacer le logo
mov ah, 0x02
int 1Ah
jc .cont

xor ax, ax
mov al, dh
inc al
mov bl, 60
div bl
mov bh, ah

.wait
mov ah, 0x02
int 1Ah
xor ax, ax
mov al, dh
mov bl, 60
div bl
cmp bh, ah
jge .wait


.cont

ret



%DEFINE fatbuffer          0x4000 ;buffer FAT12
%DEFINE buffer             0x5200 ;buffer disquette
%DEFINE file		   0x5400 ;buffer Entree de FAT12
;*****************************************;
;*      Charge un fichier à l'addresse stockée dans ax et verifie sa signature
;*****************************************;
; ENTREE :
;		ax :adresse ou sera chargé le fichier
;		di : le nom du fichier à charger
;*****************************************
;SORTIE :
;		ax = 0 : fichier non trouvé
;		ax = 1 : fichier "NON EXECUTABLE" trouvé et chargé
;		ax = 2 : fichier EXECUTABLE trouvé et chargé
;               bx : taille du fichier
;*****************************************
P_load:

push es

push ax   ; contient l'adresse ou on va charger le fichier
push ax   ; on va récuperrer cette valeure deux fois



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

;writeln tfound

mov ax, 1
call P_sl2p
mov bx, fatbuffer
mov ah, 02
mov al, 9
mov dl, 0
int 13h

pop ax
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
cmp ax, 0x0FF8
jb lo

pop bx   ;bx est toujours dans la pile on le depile


mov ax, 1  

pop es
jmp endi


NotFound:
pop ax
pop ax
mov ax, 0  ; fichier non trouv
;              writeln tnotfound
endi:

pop es
ret

;>>> PACKAGES UTILISES
;************************************************************;


%include "output.inc"
%include "video.inc"    ;**
%include "fat12.inc"
%include "vga.inc"     ;**





;************************************************************;
logoname db 'XOSLOGO BMP',0
msgno386 db "ERR : Processeur 386 non detecté..",13,10
db "Arret du système",0
