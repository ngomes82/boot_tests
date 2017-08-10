;========= CODE SEGMENT ;=========
;boot.asm
;
;nasm boot.asm -f bin -o iso_staging/boot/boot.img		 ;create floppy image! Place it in boot folder for iso.
;mkisofs -r -b boot/boot.img -o bootcd.iso iso_staging	 ;make the cd .ISO filesystem

	[ORG 0x7C00]	;Start loaded at 0000:7C00
	[BITS 16]

	xor ax, ax		;Clear AX, first boot might be all screwy.
	mov ds, ax		;Point Data segment to correct mem location (0)
	mov si, msg		;Move starting address of msg to SI register
	call biosprintstring 
	
	cli				;Disable interrupts, to start entering protected mode
	
hang:
	jmp hang			;loop the bootloader forever


biosprintstring:		;Use BIOS to pring char string in SI register
	lodsb
	or al, al
	jz done
	mov ah, al
	call biosprintchar
	jmp biosprintstring
done:
	ret



biosprintchar:			;Use BIOS to print char residing in the AL register
	mov bh, 0x0			;Write to page 0
	mov ah, 0x0E		;Use teletype output
	int 0x10			;Use BIOS interrupt 10h / ah=0x0E to draw char
	ret


;====== DATA SEGMENT ;============
msg	db 'Hello World!', 0	;Declare 0 terminated set of characters 'H', 'E', 'L', 'L', 'O', '0'
	times 510-($-$$) db 0	;BIOS looks for a 512 byte Boot Sector, subtracting from data above this line, fill up to 510 bytes with 0's
	dw 0x55AA				;Boot Signature (0x55AA) The last 2 bytes of 512 Boot Sector. Older bios's look for this.
	times 1474048 db 0		;Fill remaining space of 1.4mb floppy with 0s for El Torito Bootable CD
