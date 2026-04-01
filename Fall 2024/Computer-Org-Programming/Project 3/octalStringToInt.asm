;;=============================================================
;; CS 2110 - Spring 2024
;; Project 3 - octalStringToInt
;;=============================================================
;; Name: Jinseo Lee
;;=============================================================

;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  octalString = OCTALSTRING;
;;  length = LENGTH;
;;  value = 0;
;;  i = 0;
;;  while (i < length) {
;;      leftShifts = 3;
;;      while (leftShifts > 0) {
;;          value += value;
;;          leftShifts--;
;;      }
;;      digit = octalString[i] - 48;
;;      value += digit;
;;      i++;
;;  }
;;  mem[mem[RESULTADDR]] = value;

.orig x3000

LD R0, OCTALSTRING
LD R1, LENGTH
; R2 = i
AND R2, R2, #0
; R3 = value
AND R3, R3, #0

LOOP
NOT R4, R1
ADD R4, R4, #1
ADD R4, R2, R4
; if i - length < 0
BRzp FINISH

AND R4, R4, #0
ADD R4, R4, #3

INNER_LOOP
; if leftshift > 0
ADD R4, R4, #0
BRnz OUTER
ADD R3, R3, R3
ADD R4, R4, #-1
BR INNER_LOOP

OUTER
; R5 = octalString[i]
ADD R5, R0, R2
LDR R5, R5, #0
ADD R2, R2, #1
; octalString[i] - 48
LD R6, NEGATIVE_ASCII_ZERO
ADD R6, R5, R6
ADD R3, R3, R6
BR LOOP

FINISH
LD R7, RESULTADDR
STR R3, R7, #0
    HALT

;; You can change this for debugging!
LENGTH   .fill 4

;; Do not change these values!
OCTALSTRING         .fill x5000
RESULTADDR          .fill x4000
NEGATIVE_ASCII_ZERO .fill -48
.end

;; Your answer should be stored here!
.orig x4000
	RESULT  .blkw 1
.end

.orig x5000                    ;;  Don't change the .orig statement
    .stringz "2110"            ;;  You can change this string for debugging!
.end
