;========= CODE SEGMENT ;=========
;boot.asm
;

	[ORG 0x7C00]		;Boot starts loaded at 0000:7C00, use this so our code is 1-to-1 with physical address
	[BITS 16]

	xor ax, ax		;Clear AX, first boot might be all screwy.
	mov ds, ax		;Point Data segment to correct mem location (0)
	mov ss, ax		;Stack starts at 0
	mov sp, 0x9c00		;stack pointer, 20000h past code start
	
	mov si, msg		;Move starting address of msg to SI register
	call biosprintstring 
	
	
	cli				;Disable interrupts, to start entering protected mode
	cld				;DI is incremented when rep instruction is called
	
	;TODO: Enable the A20 line where applicable
	
	lgdt [gdtinfo]   ; load gdt register

	mov  eax, cr0   ; switch to pmode by
	or al,1         ; set pmode bit
	mov  cr0, eax
	
	jmp 08h:protected32	;long jump to the second GDT entry (8 bytes per descriptor)
[BITS 32]
protected32:
	mov		ax, 0x10		; set data segments to data selector (0x10)
	mov		ds, ax
	mov		ss, ax
	mov		es, ax

	mov bx, 0x0f01   ; attrib/char of smiley
	mov eax, 0x0b8000 ; note 32 bit offset
	mov word [ds:eax], bx

hang:
	jmp hang			;loop the bootloader forever


[BITS 16]
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
gdtinfo:
   dw gdt_end - gdt - 1   ;last byte in table
   dd gdt         ;start of table
 
gdt        dd 0,0  ; entry 0 is always unused
code_segment    db 0xff, 0xff, 0, 0, 0, 10011010b, 11001111b, 0
data_segment    db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0
gdt_end:

hexstr   db '0123456789ABCDEF'
outstr16   db '0000', 0  ;register value string
reg16   dw    0  ; pass values to printreg16
xpos db 0
ypos db 1
msg	db 'Hello World!', 0	;Declare 0 terminated set of characters 'H', 'E', 'L', 'L', 'O', '0'
	times 510-($-$$) db 0	;BIOS looks for a 512 byte Boot Sector, subtracting from data above this line, fill up to 510 bytes with 0's
	dw 0x55AA				;Boot Signature (0x55AA) The last 2 bytes of 512 Boot Sector. Older bios's look for this.
	times 1474560-($-$$) db 0 ;Fill remaining space of 1.44mb floppy image (1474560 bytes) with 0s for El Torito Bootable CD
