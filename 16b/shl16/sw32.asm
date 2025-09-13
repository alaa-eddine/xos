[BITS 16]
[ORG 0x0]
jmp start

%include "sysinfo.inc"    ;*


;>>> PACKAGES UTILISES
;************************************************************;
%include "output.inc"
%include "macros.inc"   ;**
%include "colors.inc"   ;**
%include "video.inc"    ;**
%include "mouse.inc"    ;**
%include "fat12.inc"
%include "messages.inc"    ;*
%include "tools.inc"    ;*

%DEFINE sw32sig 0xABEF


filetoload: db 0,0,0,0,0,0,0,0,0,0,0,0,0

msg db 'Passage en mode prot‚g‚...',0

msg_shell_nf db 'SW32 : ERREUR Le fichier xosker32.xss est introuvable',13,10
             db 'Retours en mode 16 bits',13,10,0

msg_shell_f db 'OK',13,10,0

shellname db 'XOSKER32XSS',0

start:
              cli
              mov    AX,9000h
              mov    SS,AX
              mov    SP,0
              sti

    mov es, ax
    mov ds, ax


writeln msg

mov di, shellname
mov ax, 100h
call P_load
cmp ax, 0
je exit
write msg_shell_f
mov ax, 100h
mov dx, sw32sig
jmp 100h:0

exit:
        write msg_shell_nf
        mov ah, 0   ;fonction 0 : retour au shell 16 bits
        int 21h
