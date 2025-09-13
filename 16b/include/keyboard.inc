%DEFINE keybuf		    0x5500;buffer clavier


;************************************************************;
;Cette procedure attend que l'utilisateur press une touche et
;renvoi le code ascii de cette derni‚re dans al
P_readkey:

;   .getkey:
;    	      mov ah, 1
;             int 16h
;             jnz .getkey

              mov ah, 0
              int 16h
              ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

P_formatkey:
              cmp [bpos], byte 254
              je .bpos0

              cmp al, 0
              je  .end
              cmp al, 8
              jne .end

              cmp [bpos], byte 0
              je  .bpos0

              call P_printch
              mov al, 32
              call P_printch
              mov al, 8
              jmp .end
.bpos0:
              mov al, 0
.end:
              ret

;*************************************************************
P_init_keybuf:
              mov [bpos], byte 0
              mov [keybuf], byte 0
              ret



;*************************************************************

P_update_keybuf:
              cmp [bpos], byte 254
              je .end

              cmp al, 8
              jne .notbackspace
              ;backspace********************
.backspace:
              cmp [bpos], byte 0
              je .al0

              dec byte [bpos]

 .al0:
              jmp .end

              ;ENDbackspace********************

   .notbackspace:

              push ax
              mov ax, 0
              mov al, [bpos]
              cbw
              mov bx, ax
              pop ax

              mov [keybuf+bx], al
              inc byte [bpos]
.end:

.ret:         ret

;*************************************************************
P_formatbuffer:
;ENTREE
;SI : buffer source
;DI : buffer cible (12 oct)
;Sortie:
;AUCUNE (buffer format‚)
         call P_size

         cmp cx, 12
         jg  .notcomfile

         mov cx, 11
         xor ax, ax
.deb:
         cmp ah, ' '
         je .nolod
         jmp .lod
.nolod:
         cmp cx, 3
         jg .nolodsb

.lod:
         lodsb
.nolodsb:
         cmp al, '.'
         je .point
         mov ah, al
         jmp .end
.point:

         cmp cx, 3
         jg .sup3
         je .eq3
         mov ah, al
         jmp .end
.sup3:
         mov ah, ' '
         jmp .end
.eq3:
         jmp .lod
.end:
         mov al, ah
         stosb
         loop .deb

         mov al, 0
         stosb

         mov cx, 1
         jmp .ret

.notcomfile:
         mov cx, 0
.ret:
         ret
