ln  db 10,13,0

%macro   write 1
  mov  si, %1
  call P_print
%endmacro

%macro   writeln 1
  mov  si, %1
  call P_print
  mov  si, ln
  call P_print
%endmacro

%macro   writenum 1
  mov  ax, %1
  call P_pr_ent
%endmacro


