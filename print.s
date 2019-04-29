; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
        
number  EQU     0
digit   EQU     4

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
    PRESERVE8

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
    PUSH {R11, LR}
;***** Allocation *****
    SUB SP, #4
    MOV R11, SP    
    CMP R0, #10
    BLO digitOutDec     ; Output a digit
    MOV R3, #10
    UDIV R2, R0, R3
    MLS R1, R2, R3, R0  ; R1 has the last digit of the number
    STR R1, [R11, #number]
    MOV R0, R2          ; Put all other digits into recurrsion
    BL LCD_OutDec
    LDR R0, [R11, #number]
digitOutDec
    ADD R0, #0x30
    BL ST7735_OutChar
    
    ADD SP, #4
    POP {R11, PC}
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
    PUSH {R11, LR}
    SUB SP, #8
    MOV R11, SP
    
    MOV R1, #9999
    CMP R0, R1
    BHI printStars
    
    STR R0, [R11, #number]
    MOV R2, #1000
    UDIV R1, R0, R2
    STR R1, [R11, #digit]
    MOV R0, R1
    BL LCD_OutDec
    MOV R0, #0x2E
    BL ST7735_OutChar
    LDR R0, [R11, #number]
    LDR R1, [R11, #digit]
    MOV R2, #1000
    MLS R0, R1, R2, R0
    STR R0, [R11, #number]
    MOV R1, #10
    
    CMP R0, #0
    BNE checkSize
    MOV R0, #0x30
    BL ST7735_OutChar
    MOV R0, #0x30
    BL ST7735_OutChar
    MOV R0, #0x30
    BL ST7735_OutChar
    B finish

checkSize
    CMP R0, #100
    BHS printFix 
    MOV R0, #0x30
    BL ST7735_OutChar
    LDR R0, [R11, #number]
    CMP R0, #10
    BHS printFix
    MOV R0, #0x30
    BL ST7735_OutChar
    LDR R0, [R11, #number]
    B printFix
    
    B checkSize
printFix
    BL LCD_OutDec
    B finish
    
printStars
    MOV R0, #0x2A
    BL ST7735_OutChar
    MOV R0, #0x2E
    BL ST7735_OutChar
    MOV R0, #0x2A
    BL ST7735_OutChar
    MOV R0, #0x2A
    BL ST7735_OutChar
    MOV R0, #0x2A
    BL ST7735_OutChar
    
finish
    ADD SP, #8
    POP {R11, PC}
;* * * * * * * * End of LCD_OutFix * * * * * * * *

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
