
col        DB 0
lin        DB 0
bufpos     DB 0
PAL      dw 0
R        db 0
G        db 0
B        db 0


;-------positionnement du curseur (GOTOXY)
;dh := ligne
;dl := colonne
P_setcurpos:
              push ax
              push bx
              mov ah, 2
              mov bh, 0
              int 10h
              pop bx
              pop ax
              ret


P_getcurpos:
              push ax
              push bx
              mov ah, 3
              mov bh, 0
              int 10h
              mov [lin], dh
              mov [col], dl
              pop bx
              pop ax
              ret

;-------reinitialisation du mode texte (80x25)
P_inittext:
              push ax
              push bx

              mov ah, 0
              mov al, 03h
              int 10h

;              mov bl, [Red]
;              mov ah, 11
;              mov bh,0
;              int 10h

              pop bx
              pop ax
              ret


;-------effacer l'ecran
P_cls:
              push dx
              push cx
              push bx
              push ax

              mov ah, 6
              mov al, 50
              mov ch, 0
              mov cl, 0
              mov dh, 49
              mov dl, 79
              int 10h

      mov ax, 1112h     ;charge le jeu de caractères 8*8 du BIOS
      mov bl, 0         ;dans la table principale de caractères
      int 10h           ;ce qui permet d'avoir une résolution de 80x50 en mode text ;-)

              mov dx, 0000h
              call P_setcurpos

              pop ax
              pop bx
              pop cx
              pop dx
              ret

;------couleur de fond
P_setbkcolor:
              push ax
              push bx

              mov ah, 11
              mov bh, 0
              int 10h

              pop bx
              pop ax
              ret

;---------------------------------
setpal : ;modifie une entr‚e de la pallette DAC
push ax
push bx
push cx
push dx



push bx
push dx



xor ax, ax
mov al, cl
mov bx, 63
mul bx
mov bx, 255
div bx
mov cl, al

xor ax, ax
mov al, ch
mov bx, 63
mul bx
mov bx, 255
div bx
mov ch, al


pop dx
xor ax, ax
mov al, dh
mov bx, 63
mul bx
mov bx, 255
div bx
mov dh, al


pop bx
mov ax, $1010
int 10h	

pop dx
pop cx
pop bx
pop ax
ret


