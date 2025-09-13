; Procédure qui permet d'allumer un pixel dans la mémoire vidéo
; au format VGA en mode réel.

; Bombela - supercar@tiscali.fr - 24/03/2004

; Syntaxe > NASM.

; Format d'appel: SYSCAll.


; **************************************************************
PutPixel:

; WORD: X et Y l'emplacement du point.
; WORD: Larg Largeur d'une ligne d'écran.
; BYTE: Code couleur du point.
; ---------------------------------------------------------------

%define Color bp+4 ; BYTE
%define Larg bp+6 ; WORD
%define Y bp+8 ; WORD
%define X bp+10 ; WORD

enter 0,0 ; Un cadre de pile.
  pushad ; Sauv les regs.

mov ax, [Y] ; Multiplication de Y
mov bx, [Larg] ; par la largeur de l'affichage.
mul bx ; Le résultat se trouve dans dx:ax

shl edx, 16 ; Recolle dx:ax
mov dx, ax ; dans edx.

movzx eax, word [X] ; Ajoute X
add edx, eax ; à edx.

push edx ; Sauv edx.
shr edx, 16 ; Récupère dans dx le mot de poid fort de EDX.

cmp dx, [cs:SauvWin] ; DX indique donc la fenêtre de la ram vidéo.
je .suit	; La variable SauvWin contient la fenêtre courante.
		; Si la fenêtre est déjà active, on passe.

mov [cs:SauvWin], dx ; Actualise SauvWin avec la fenêtre calculée.
mov ax, 4f05h	; Demande au BIOS de changer
xor bx, bx ; la bank vidéo active.
int 10h ; Ce qui l'incruste à l'adresse de video VGA.

.suit

pop ebx ; On reprend l'adresse absolue du pixel.

mov ax, 0A000h ; L'adresse de la ram vidéo
mov es, ax     ; dans le reg de seg es.
mov al, [Color] ; On charge dans al la couleur du pixel.
mov [es:bx], al ; Et comme bx contient l'offset du pixel dans la bank courante,
		; il suffit d'envoyer la couleur à es:bx.

  popad ; Remet en état les regs.
leave ; Plus de cadre de pile.

ret 8 ; Pour un format d'appel en C, retirer le 8.

SauvWin dw 0

;-----------------------------------------------------
