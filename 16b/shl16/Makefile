###########################################################################
# X-OS
#
#
###########################################################################
# X-OS Makefile by Alaa-eddine KADDOURI
# https://xos.eurekaa.org
###########################################################################
#
# "16b/shl16/Makefile"
#

OBJ=shell16
BINDIR=../../build/obj

all: $(OBJ)

shell16:
	nasm -i ../include/ -f bin shl16.asm -o shl16.xss
	nasm -i ../include/ -f bin sw32.asm -o sw32.xss
	nasm -i ../include/ -f bin prog1.asm -o prog1.com

	mv *.xss *.com $(BINDIR)


clean:
	rm -f  *.com *.xss
