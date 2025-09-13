jmp start

sig	db 'XBIN16',0	; signature XBIN  : pour que le fichier puisse etre executé par le shell il doit contenir cette signature
				;			   qui indique que c'est un fichier executable en mode 16 bits

msg1: db 'Bonjours Je suis le 1er programme lanc‚ par X-OS...',10,13,0
;      db 'Au revoir ...',0


start:
    mov ax, 06000h
    mov es, ax
    mov ds, ax

   mov si, msg1
    mov ah, 09h ;fonction 09h : affichege ASCIIZ
    int 21h


    mov ah, 0   ;fonction 0 : retour au shell
    int 21h
    ret
