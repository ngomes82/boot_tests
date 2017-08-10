# boot_tests

Boot from Fat32 USB Example:
https://github.com/ishanthilina/USB-FAT32-Bootloader/blob/master/src/boot.asm

Barebones MBR to possibly boot from USB:
http://wiki.osdev.org/MBR

Tests for rolling a custom bootloader
http://wiki.osdev.org/Babystep2



Build commands used in Cygwin:

nasm boot.asm -f bin -o iso_staging/boot/boot.img   				 #create floppy image! Place it in boot folder for iso.

mkisofs -r -b boot/boot.img -o bootcd.iso iso_staging				 #make the cd .ISO filesystem
