###########################################################################
# X-OS
#
#
###########################################################################
# X-OS Makefile by Alaa-eddine KADDOURI
# https://xos.eurekaa.org
###########################################################################
#
# "16b/kernel/Makefile"
#

OBJ=kernel
BINDIR=../../build/obj
all: $(OBJ)

kernel:
	nasm -i ../include/ -f bin xosker16.asm -o xosker16.xss
	mv xosker16.xss $(BINDIR)


clean:
	rm -f $(OBJ) *.xss
