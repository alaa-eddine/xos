%include 'putpixvga.inc'





P_aff_grlogo:      ;affichage du logo X-OS


vga:

mov ah, 0
mov al, 13h
int 10h



mov si, word 54
mov bx, 0          ;premiŠre entr‚e dans la palette 256 couleurs

mov dx, 0
mov cx, 256

cld
lodsb
push es
push di


mov ax, seg_palette
mov es, ax
mov di, 0

.set_pal_loop:
push cx

mov cl, al
	xor ax, ax
	mov al, cl
	mov bx, 63
	mul bx
	mov bx, 255
	div bx
;stosb
push ax


lodsb
mov ch, al
	xor ax, ax
	mov al, ch
	mov bx, 63
	mul bx
	mov bx, 255
	div bx
;stosb
push ax

lodsb
mov dh, al
	xor ax, ax
	mov al, dh
	mov bx, 63
	mul bx
	mov bx, 255
	div bx
stosb
pop ax
stosb
pop ax
stosb



inc bx
lodsb
lodsb
pop cx
loop .set_pal_loop

dec si
.nopal:



mov ax, 1012h
mov bx, 0
mov cx, 255
mov dx, 0
int 10h

pop di
pop es
;;;;;;;;;;;;;
l0:

mov dx, word 199  ;149   hauteur+decalage-1
mov cx, word 0    ;80     position à gauche
._deb:
cmp si, 65078
je  ._end

mov bx, si
mov al, byte [bx]


xchg al, bl

;mov ax, 320
;mov bh, 0
;push cx
;push dx
;push ax
;push bx
;call PutPixel


mov ah, 12
mov al, bl
int 10h

inc word si
inc cx
cmp cx, word 320; 160  ;240 largeur+décalage à gauche
jge  ._egale

jmp ._deb

._egale:
mov cx, 0   ;80   décalage à gauche
dec dx

jmp  ._deb

._end:


v_end:
endvga:


ret
