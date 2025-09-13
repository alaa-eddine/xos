

;**********************************************************************
;*                      GNU X-OS Boot loader                          *
;**********************************************************************
;* @Auteur : Alaa-eddine KADDOURI                                     *
;*                                                                    *
;* @Projet : X Operating System (X-OS)                                *
;* @version : XOS LOADER v3.0                                         *
;*                                                                    *
;* @Description : ce code permet de charger deux fichiers, un setup   *
;* et un noyeau, puis lance le setup qui doit passer en mode          *
;* protégé déplacer le noyeau au dela du premier Mo puis le lancer    *
;*								      *
;* @Site web : https://xos.eurekaa.org       *
;**********************************************************************


[ORG 0x7C00]
[BITS 16]

jmp  start     ;jmp to executable code 3bytes

;**********************************************************************
;* boot record : NE PAS MODIFIER                                      *
;**********************************************************************
_OEM_NAME__________    db 'XoLo3',10,13,0
_bytes_per_sector__    dw 512
_sect_per_cluster__    db 1
_reserved_sectors__    dw 1
_number_of_FATs____    db 2
_number_of_entries_    dw 224
_nb_total_sectors__    dw 2880
_media_descriptor__    db 0xF0
_sectors_per_FAT___    dw 9
_sector_per_track__    dw 18
_number_of_heads___    dw 2
_nb_hidden_sectors_    dw 0
_nb_part_sectors___    dd 0
_drive_number______    db 0
_reserved1_________    db 0
_signature_________    db 29h
_serial_number_____    dd 0
;_volume_label______    db 'XOS-LOADER ' ;11bytes
;_reserved2_________    db 0, 0, 0, 0, 0, 0, 0, 0 ;8bytes
;**********************************************************************
;**********************************************************************
;les outils pour la fat12 et l'affichage
%include "boot.inc"


setup	      db 'XOSKER32'
kername       db 'SETUP   XSS',0   ;le format du nom ici est en 8x3
;exemple: 'NOMFIC00EXT'



%define MSG_init  _OEM_NAME__________    
;              db 'By AlAdDiN',10,13


;***CONST et var
%define bootdrive  0

;***quelques adesses memoires utiles	      
LOAD_ADDR DW 9020h


%define buffer 0x7e00
%define file   0x8000


;***MESSAGES

MSG_pix	  db '.',0
MSG_ok     db 'OK',10,13,0
MSG_notfound  db 'E',0
;MSG_reboot    db 'Hit key',10,13,0



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
start:
cli
mov    AX,9000h
mov    SS,AX
mov    SP,0h
sti


mov es, ax
push ax
xor ax, ax
mov ds, ax

mov ax, 7c00h
mov si, ax
mov di, ax

mov cx, 0100h
rep movsw


mov ax, go
push ax

retf




go:

mov ax, cs
mov ds, ax
mov es, ax




mov si,  _OEM_NAME__________
call P_print

load:
mov si, kername
call P_print

mov ax, 19			;premier secteur de la fat
lo1:
push ax				;on sauveguarde ax (compteur de boucle principalle)
mov bx, buffer			;le buffer dans lequel on va lire les secteur de la fat

Call  P_readsector		;lecture d'un secteur
mov cx, 16			;16 entrées dans chaque buffer de 512 octets
mov dl, 1				;1ere entrée de la fat
lo2:
push cx				;on sauvegare cx (car il servira de compteur de boucle)
mov di, file			;buffer qui contiendra une entrée de la fat (32 octets) (chaque entrée contient le nom du fichier, les attribut, la taille ...etc)
mov bx, buffer			;le buffer qui contien l'entré à lire
call P_buf2file			;on lit l'entrée
mov bx, file			;
call P_decompfile		;extractions des données de cet entrée (nom du fichier, attributs, dates, taille...)

mov  si, filename		;filename est une chaine qui contien le nom du dernier ficher extrait de la fat (voir boot.inc)
mov  di, kername		;le nom de notre kernel
mov  cx, 11			;8 char pour le nom 3 pour l'extention = 11
repe cmpsb			;comparaison des deux chaines
jz   Found				;Youpiii  ............... On a trouvé le kernel !!!

next1:					;pas encore ??
inc  dl				;entré suivante de la fat

pop cx				;on recupère la valeur de notre compteur
loop lo2				;puis on relance la recherche du kernel
pop  ax				;on recupère la valeur de notre compteur principal
cmp  ax, 32			;toute la fat à était lu ??
jge  NotFound			;oui ===>   le kernel n'est pas la d'dans   :-(

		inc  ax				;ya encore des entrées dans la fat ?
		jmp  lo1				;oui , on passe à la suivante


		Found:					;le kernel est trouvé
		pop cx				;d'abord on libère la pile
		pop ax

		mov ax, 1				;
		call P_sl2p			;secteur logique en physique
		mov bx, buffer
		mov ah, 02
		mov al, 9
		mov dl, bootdrive
		int 13h

		mov bx, 0
		push bx

		mov ax, [LOAD_ADDR]
mov es, ax


lo:						;chargement du kernel
mov si, MSG_pix
Call P_print

mov ax, [cluster]
pop bx
add ax, 31
call P_readsector
add bx, 512
push bx


mov bx, buffer
mov ax, [cluster]
call P_getnextcluster
mov  [cluster], ax

cmp ax, 0x0FF8			;fin du fichier
jb lo

mov si, MSG_ok		;le kernel est chargé
Call P_print



cmp word [LOAD_ADDR], 9020h
jne finish

mov ax, ds
mov es, ax
mov si, setup
mov di, kername
mov cx, 8
rep movsb
mov [LOAD_ADDR], word 100h
jmp load

finish:	





mov ax, word 0x9020 	;on sauveguarde l'addresse de chargement du kernel

jmp 0x9020:0		;lancement du kernel...


NotFound:
mov si, MSG_notfound
call P_print

mov ah, 0 ;attente d'une touche
int 16h   ;

int 19h
jmp $



times 510 - ($-$$)  DB 0
MAGIC_NUMBER        dw 0AA55h

[SEGMENT .bss]		
;file     resb 32
;buffer resb 512*9

filename resb 8
ext resb 3
