# boot_tests

Tests for rolling a custom bootloader
http://wiki.osdev.org/Babystep2


Build Commands Used In Cygwin

nasm boot.asm -f bin -o iso_staging/boot/boot.img   				 #create floppy image! Place it in boot folder for iso.

mkisofs -r -b boot/boot.img -o bootcd.iso iso_staging				 #make the cd .ISO filesystem
