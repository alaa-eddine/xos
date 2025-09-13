

%define seg_palette 0x4000

MSG_getvesa:     DB 'detection de la carte video...',0
MSG_errvesa:     DB 'Aucune carte VESA d‚t‚ct‚e',0

MSG_construct:   DB 'Id Carte video : ',0

MSG_VRAM:        DB 'Taille de la memoire video : ',0
MSG_sig:         DB 'Signature VESA : ',0
MSG_vesaver:     DB 'Version VESA : ',0


;*****************VESA block********************
VBEINFO:
 VBE00h  times 4 db 0   ;(ret) signature ("VESA")
		        ;(call) VESA 2.0 request signature ("VBE2"), required to receive
		        ;  version 2.0 info
 VBE04h	dw 0	        ;VESA version number (one-digit minor version -- 0102h = v1.2)
 VBE06h1	dw 0	;pointer to OEM name (ofs)
 VBE06h2	dw 0	;pointer to OEM name (seg)
		        ;"761295520" for ATI
 VBE0Ah	dd 0	        ;capabilities flags (see #00078)
 VBE0Eh	dd 0       	;pointer to list of supported VESA and OEM video modes
	           	;(list of words terminated with FFFFh)
 VBE12h	dw 0       	;total amount of video memory in 64K blocks

;---VBE v2.0 ---
 VBE14h	dw 0       	;OEM software version (BCD, high byte = major, low byte = minor)
 VBE16h	DD 0       	;pointer to vendor name
 VBE1Ah	DD 0       	;pointer to product name
 VBE1Eh	DD 0       	;pointer to product revision string
 VBE22h	Dw 0       	;(if capabilities bit 3 set) VBE/AF version (BCD)
	           	;0100h for v1.0P
 VBE24h	DD 0       	;(if capabilities bit 3 set) pointer to list of supported
		        ;  accelerated video modes (list of words terminated with FFFFh)
 VBE28h times 216 db 0	;reserved for VBE implementation
 VBE100h times 256 db 0	;OEM scratchpad (for OEM strings, etc.)

;*************END OF VESA block*****************

;***************************************************************************
;***************************************************************************


;*** GET VESA INFO *******************************************
P_vesainfo:
         mov ax, $4f00
         mov di, VBEINFO
         int $10
         ret

putpixel:
;ENTREE
;ax = ligne
;bx = col
;ch = couleur
;;;;;;;;;;;
;SORTIE
push ds

push cx
mov cx, 640
mul cx

add bx, ax
pop cx

mov ax, 0a000h
mov ds, ax
mov [ds:bx], byte ch
pop ds
ret

;************************************************************;

;***VESA
P_print_vesainfo:  ;affichage des informations VESA

call P_vesainfo    ;recuperation des information VESA dans la table
                   ;VESA block

writeln MSG_getvesa;
cmp ah, 00h

jne vsaerr

;write MSG_sig
;writeln VBE00h     ;signature VESA


write MSG_vesaver  ;
mov bx, [VBE04h]     ;version vesa
mov al, bh         ;
cbw                ;
writenum ax        ;
write pix          ;
mov al, bl         ;
cbw                ;
writenum ax        ;
write ln           ;

write MSG_construct
push ds
lds si, [VBE06h1]  ;nom du constructeur
call P_print
pop ds

write ln
mov ax, [VBE12h]
mov cx, 64
mul cx
mov cx, 1024
div cx


write MSG_VRAM
writenum ax        ;taille de la VRAM
write MSG_MO

jmp endvesainfo

vsaerr:
writeln MSG_errvesa


endvesainfo:
;***VESA
ret





P_vesa_grlogo:      ;affichage du logo X-OS


;***VESA
Vesa:
call P_vesainfo    ;recuperation des information VESA dans la table
                   ;VESA block

cmp ah, 00h

je  _go
jmp err


_go:
mov	ax,$4f02
mov	bx,101h
int	10h
mov	ax,$4f05
mov	bx,$0000
mov	dx,$0000
int	10h

;mov ax, 0
;mov bx, 0
;mov ch, 255
;call putpixel

mov bl, 1

mov si, 0000h

add si, word 54



mov bx, 0          ;premiŠre entr‚e dans la palette 256 couleurs
;mov [PAL], word 0



mov cx, 256        ;256 couleurs donc 256 tour de boucle

set_pal_loop:
push cx

push bx            ; on sauveguarde la position de bx dans la palette
                   ; car bx va etre utiliser pour faire d'autres calculs


;mov bx, word [gradr]
mov bx, word si
mov cl, [bx]
;inc word [gradr]
inc word si

;mov bx, word [gradr]
mov bx, word si
mov ch, [bx]
;inc word [gradr]
inc word si

;mov bx, word [gradr]
mov bx, word si
mov dh, [bx]
;inc word [gradr]
inc si


pop bx             ; on recupŠre bx
call setpal        ; puis on met a jours l'entr‚e dans la palette

;inc word [gradr]
inc si

inc bx             ; on passe a l'entree suivante de la palette
;inc word [PAL]

pop cx
loop set_pal_loop




mov bl, 1
mov [grcol], word 0
mov [grlin], word 99 ;100-1
_deb:

cmp si, 17078
jge  _end

mov bx, si
mov al, byte [bx]
xchg al, bl

mov ch, bl
mov ax, [grlin]
mov bx, [grcol]
call putpixel


inc si

inc word [grcol]
cmp [grcol], word 160  ;160
je  _egale
jmp _deb

_egale:
mov [grcol], word 0
dec word [grlin]
jmp _deb

_end:

;call P_readkey


jmp endvesa

err:
mov ah, 0
mov al, 3h
int 10h

writeln MSG_errvesa

endvesa:
call P_readkey
mov ah, 0
mov al, 3h
int 10h

jmp v_end
;***VESA

ret


