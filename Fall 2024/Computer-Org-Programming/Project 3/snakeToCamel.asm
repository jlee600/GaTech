;;=============================================================
;; CS 2110 - Fall 2024
;; Project 3 - Snake to Camel Case
;;=============================================================
;; Name: Jinseo Lee
;;=============================================================

;;  capitalizeNext = 0;
;;  underscore = UNDERSCORE;
;;  length = LENGTH;
;;  snakeString = SNAKESTRING;
;;  camelIndex = 0;
;;  camelString = RESULTADDR;
;;  for(i = 0; i < length; i++) {
;;      char = mem[snakeString + i];
;;      if (char == underscore) {
;;          capitalizeNext = 1;
;;      } else {
;;          if (capitalizeNext == 1) {
;;              mem[camelString + camelIndex] = toUpper(char);
;;              capitalizeNext = 0;
;;              camelIndex++;
;;          }  else {
;;               mem[camelString + camelIndex] = char;
;;               camelIndex++;
;;          }
;;      }
;;  }
;;  mem[camelString + camelIndex] = '\0';

.orig x3000
LD R0, UNDERSCORE
LD R1, LENGTH
LD R2, SNAKESTRING
LD R3, RESULTADDR ; R3 = camelString
; R4 = capitalizeNext
AND R4, R4, #0
; R5 = camelIndex
AND R5, R5, #0
; R6 = i
AND R6, R6, #0

LOOP
; reset registers
LD R0, UNDERSCORE
LD R1, LENGTH
LD R2, SNAKESTRING
LD R3, RESULTADDR

; while i - length < 0
NOT R7, R1
ADD R7, R7, #1
ADD R7, R6, R7
BRzp FINISH

; char = mem[snakeString + i];
ADD R7, R2, R6
LDR R7, R7, #0 ; R7 = char

; if (char - underscore) == 0
NOT R0, R0
ADD R0, R0, #1
ADD R7, R7, R0 ; R7 = char - underscore
BRnp OUTER_ELSE
and r4,r4, #0
ADD R4, R4, #1 ; R4 = captializeNext = 1
ADD R6, R6, #1
BR LOOP

; else
OUTER_ELSE
; retrieve char again
ADD R7, R2, R6
LDR R7, R7, #0 ; R7 = char

; if (capitalizeNext - 1) == 0
ADD R4, R4, #-1
BRnp INNER_ELSE
; mem[camelString + camelIndex] = toUpper(char)
ADD R3, R3, R5
; toUpper(char)
ADD R7, R7, #-15
ADD R7, R7, #-15
ADD R7, R7, #-2
STR R7, R3, #0
ADD R5, R5, #1
ADD R6, R6, #1
BR LOOP

INNER_ELSE
; mem[camelString + camelIndex] = char
ADD R4, R4, #1 ; R4 = captializeNext = 1
ADD R3, R3, R5
STR R7, R3, #0
ADD R5, R5, #1
ADD R6, R6, #1
BR LOOP

FINISH
;  mem[camelString + camelIndex] = '\0'
ADD R3, R3, R5
AND R0, R0, #0
STR R0, R3, #0
    HALT
    
    SNAKESTRING .fill x4000
    LENGTH .fill 16
    RESULTADDR .fill x5000
    UNDERSCORE .fill 95
.end

;;  String value - you can change this for debugging!
;;  If you do, remember to change LENGTH accordingly!
.orig x4000
    .stringz "snake_case_extra"
.end
