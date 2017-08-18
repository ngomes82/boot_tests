# boot_tests

Really good Info:
http://www.brokenthorn.com/Resources/OSDev9.html

Boot from Fat32 USB Example:
https://github.com/ishanthilina/USB-FAT32-Bootloader/blob/master/src/boot.asm

Stack Overflow Answer on bootables:
https://stackoverflow.com/questions/22054578/how-to-run-a-program-without-an-operating-system/32483545#32483545

Barebones Bootloader/Multiboot examples:
https://github.com/cirosantilli/x86-bare-metal-examples/tree/d217b180be4220a0b4a453f31275d38e697a99e0

Barebones MBR to possibly boot from USB:
http://wiki.osdev.org/MBR

Tests for rolling a custom bootloader
http://wiki.osdev.org/Babystep2


Math reference:
http://www.euclideanspace.com/maths/geometry/rotations/conversions/index.htm

Build commands used in Cygwin:

--- CD ISO (needs last line in boot.asm for sizing)

nasm boot.asm -f bin -o iso_staging/boot/boot.img   				 #create image! Place it in boot folder for iso.

mkisofs -r -b boot/boot.img -o bootcd.iso iso_staging				 #make a cd .ISO filesystem

---Floppy---

nasm boot.asm -f bin -o floppy.bin

dd bs=512 count=2880 if=floppy.bin of=floppy.img     # make a floppy image
