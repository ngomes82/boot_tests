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
	
	
	mov ax, 0xe0fa		   ;test contents
	mov  word [reg16], ax  ;look at register contents
	mov ah, 0xf0		   ;white on black
	call vmemprintreg16
	
hang:
	jmp hang			;loop the bootloader forever


;----- Procedure vmemprintstring -----
; Prints a null terminated string. Advances
; the x and y writer position for future calls
;
; ES = Absolute address for start of video memory
; AH = Formatting attributes for characters to print
; SI = Pointer to starting address of string to print

vmemprintstring_doloop:   
	call vmemprintchar         ; print one character
	
vmemprintstring:
	lodsb			;load string char to al
	cmp al, 0
	jne vmemprintstring_doloop
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


;----- Procedure printreg16 -----
; Use video memory to print the contents of 
; a 16 bit register in hexadecimal
;
; reg16 = register contents to look at
; AH	= formatting attributes for characters
vmemprintreg16:
	mov dh, ah 			;store formatting data
	mov di, outstr16
	mov ax, [reg16]
	mov si, hexstr
	mov cx, 4   		;four hex characters to fill
vmemprintreg16_loop:
	rol ax, 4   		;Rotate four bits over
	mov bx, ax   		;Move to bx to avoid altering ax contents
	and bx, 0x0f   		;AND lower 4 bits with (0b00001111, 0d15) to arrive at index from 0-14 for hex char
	mov bl, [si + bx]	;Use the index to reference hexstr
	mov [di], bl		;Move hex char into output
	inc di
	dec cx
	jnz vmemprintreg16_loop

	mov ah, dh		   ;Restore formatting attributes
	mov si, outstr16   ;Move output hex characters to be drawn
	call vmemprintstring

	ret
;----- End Procedure -----


;----- Procedure biosprintstring -----
;Use BIOS cpu interrupt to print a string (null terminated)
;
; SI = address of the starting string

biosprintstring:		
	lodsb
	or al, al
	jz biosprintstring_done
	mov ah, al
	call biosprintchar
	jmp biosprintstring
biosprintstring_done:
	ret
;----- End Procedure -----


;----- Procedure biosprintchar -----
;Use BIOS to print a single char
;
;AL = ASCII character to print

biosprintchar:			
	mov bh, 0x0			;Write to page 0
	mov ah, 0x0E		;Use teletype output
	int 0x10			;Use BIOS interrupt 10h / ah=0x0E to draw char
	ret
;----- End Procedure -----


;====== DATA SEGMENT ;============
hexstr   db '0123456789ABCDEF'
outstr16   db '0000', 0  ;register value string
reg16   dw    0  ; pass values to printreg16
xpos db 0
ypos db 1
msg	db 'Hello World!', 0	;Declare 0 terminated set of characters 'H', 'E', 'L', 'L', 'O', '0'
	times 510-($-$$) db 0	;BIOS looks for a 512 byte Boot Sector, subtracting from data above this line, fill up to 510 bytes with 0's
	dw 0x55AA				;Boot Signature (0x55AA) The last 2 bytes of 512 Boot Sector. Older bios's look for this.
	times 1474560-($-$$) db 0 ;Fill remaining space of 1.44mb floppy image (1474560 bytes) with 0s for El Torito Bootable CD
