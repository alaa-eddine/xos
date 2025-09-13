;****************************************************************************;
;*                              -=[ POCEDURES ]=-                           *;
;****************************************************************************;

;*****************************************;
;*      REDEMARRAGE DE L'ORDINATEUR      *;
;*****************************************;
s_reboot :
                        mov ax, 40h        ;
                        mov es, ax         ;
                        mov di, 72h        ;
                        mov ax, 1234       ;
                        stosw              ;
			jmp 0FFFFh:0000h   ;
                        ret




;************************************************************;
;cette procedure affiche la date de la derniere compilation
;du kernel
P_print_last_compil_date:

         write     MSG_lastcompil
         mov al,   [compilday]
         cbw
         writenum  ax
         write     slash
         mov al,   [compilmonth]
         writenum  ax
         write     slash
         writenum  [compilyear]
         write     ln
         ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;************************************************************;
;cette procedure affiche le numero de version du kernel
P_print_version_number:
         mov al, [version]
         cbw
         writenum  ax
         write     pix
         mov al, [subver1]
         cbw
         writenum  ax
         write     pix
         mov al, [subver2]
         cbw
         writenum  ax
         write space
         ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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


         push di   ; contient le nom du fichier … charger

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


         mov ax, 1  ; fichier trouv‚ , il faut vérifier s'il est executable

;***************************** Verification de la signature XBIN16
	pop es
	mov di, 3
	mov si, sig
	mov cx, 7
	rep cmpsb
	je .eq
	jmp .neq
.eq
	mov ax, 2; ax contient maintenant la valeur 2 qui signifie que le fichier existe et est executable par ce shell
.neq

	 jmp endi


NotFound:
              pop ax
              pop ax
              mov ax, 0  ; fichier non trouv‚
;              writeln tnotfound
endi:
	     pop es
             ret

sig	db 'XBIN16',0
