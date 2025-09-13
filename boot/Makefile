#
# "boot/Makefile"
#

BINDIR=../build/obj
ASM=nasm
AFLASGS= -D DO_SPLASH

all: boot setup

boot: boot.asm
	$(ASM) -f bin boot.asm -o boot
	mv boot $(BINDIR)


setup: 	setup.asm
	$(ASM) $(AFLASGS) -i ../16b/include/ -f bin $^ -o setup.xss
	mv *.xss $(BINDIR)
