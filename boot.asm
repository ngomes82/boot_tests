;========= CODE SEGMENT ;=========
;boot.asm
;
;nasm boot.asm -f bin -o iso_staging/boot/boot.img		 ;create floppy image! Place it in boot folder for iso.
;mkisofs -r -b boot/boot.img -o bootcd.iso iso_staging	 ;make the cd .ISO filesystem

	[ORG 0x7C00]	;Start loaded at 0000:7C00
	[BITS 16]

	xor ax, ax		;Clear AX, first boot might be all screwy.
	mov ds, ax		;Point Data segment to correct mem location (0)
	mov ss, ax		;Stack starts at 0
	mov sp, 0x9c00	;20000h past code start
	mov si, msg		;Move starting address of msg to SI register
	call biosprintstring 
	
	cli				;Disable interrupts, to start entering protected mode
	cld				;DI is incremented when rep instruction is called
	
	;----------------------------------------------
	;Draw a single red square at 0,0 as an example of video memory drawing
	mov ax, 0xb800   ; look at video mem
	mov es, ax		 
	mov ax,0x4020 	 ;colour + space character(0x20)
	xor di, di		 ;DI position 0,0
	stosw			 ;Move contents of AX into Address located in ES
	;----------------------------------------------
	
	mov ax, 0xb800  ;Move to Video Mem
	mov es, ax
	mov ah, 0x04	;Red on black
	mov si, msg
	call vmemprintstring
	
	mov ax, 0xb800  ;Move to Video Mem
	mov es, ax
	mov ah, 0x05	;Pink on black
	mov al, 0x4d	;'M' char
	call vmemprintchar
	
	mov ax, 0xb800  ;Move to Video Mem
	mov es, ax
	mov ah, 0x06	;Orange on black
	mov al, 0x4f	;'O' char
	call vmemprintchar
	
	
hang:
	jmp hang			;loop the bootloader forever


;----- Procedure vmemprintstring -----
; Prints a null terminated string. Advances
; the x and y writer position for future calls
;
; ES = Absolute address for start of video memory
; AH = Formatting attributes for characters to print
; SI = Pointer to starting address of string to print
dochar:   
	call vmemprintchar         ; print one character
	
vmemprintstring:
	lodsb			;load string char to al
	cmp al, 0
	jne dochar
	add byte[ypos], 1 ;down one row (LF)
	mov byte[xpos], 0 ;back to left (CR)
	ret
;----- End Procedure -----
	

;----- Procedure vmemprintchar -----
; Prints a single character. Advances the
; x writer position.
;
; ES = Absolute address for start of video memory
; AL = ASCII Character to print
; AH = Formatting attributes for character
vmemprintchar:
	mov cx, ax		  	 ;Save char/attribute
	movzx ax, byte[ypos] ;grab ypos
	mov dx, 160			 ;2 bytes (char/attrib)
	mul dx				 ;80 columns
	movzx bx, byte[xpos] ;grab xpos to bx
	shl bx, 1			 ;times 2 to skip attrib
	
	mov di, 0			 ;start of video mem
	add di, ax			 ;add yoffset
	add di, bx			 ;add xoffset
	
	mov ax, cx			 ;restore char/attrib
	stosw				 ;write char/attribute
	add byte[xpos], 1	 ;advance right
	ret
;----- End Procedure -----

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
xpos db 0
ypos db 1
msg	db 'Hello World!', 0	;Declare 0 terminated set of characters 'H', 'E', 'L', 'L', 'O', '0'
	times 510-($-$$) db 0	;BIOS looks for a 512 byte Boot Sector, subtracting from data above this line, fill up to 510 bytes with 0's
	dw 0x55AA				;Boot Signature (0x55AA) The last 2 bytes of 512 Boot Sector. Older bios's look for this.
	times 1474560-($-$$) db 0 ;Fill remaining space of 1.44mb floppy image (1474560 bytes) with 0s for El Torito Bootable CD
