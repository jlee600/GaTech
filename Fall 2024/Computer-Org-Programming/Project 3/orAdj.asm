;;=============================================================
;; CS 2110 - Fall 2024
;; Project 3 - OR Adjacent Elements
;;=============================================================
;; Name: Jinseo Lee
;;=============================================================

;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  n = LENGTH;
;;  result = TARGET;
;;  nums = ARRAY;
;;  for (i = 0; i < n - 1; i++) {
;;      result[i] = nums[i] | nums[i + 1];
;;  }

.orig x3000
; R0 = mem[LENGTH]
LD R0, LENGTH
; R1 = &firstElement
LD R1, TARGET
; R2 = arrStart = mem[ARRAY]
LD R2, ARRAY
; R3 = i
AND R3, R3, #0

LOOP
; if (i-n+1) < 0
NOT R4, R0
ADD R4, R4, #1
ADD R4, R3, R4
ADD R4, R4, #1
BRzp FINISH

; arrAddr1 = arrStart + i
ADD R5, R2, R3
; currNum = mem[arrAddr1]
LDR R4, R5, #0
; arrElement2 = arrAddr1 + 1
ADD R5, R5, #1
; nextNum = mem[arrAddr2]
LDR R5, R5, #0

    ; R4 | R5 
    NOT R4, R4
    NOT R5, R5
    AND R4, R4, R5
    NOT R4, R4
    ; R5 = arrStart + i
    ADD R5, R1, R3
    ; mem[arrStart + i] = R4
    STR R4, R5, #0
    
; i++
ADD R3, R3, #1
BR LOOP

FINISH
    HALT
    
;;  You can change this for debugging!
LENGTH .fill 4

;; Do not change these values!
ARRAY  .fill x4000
TARGET .fill x4100
.end


;;  Array values - you can change these for debugging!
;;  Remember to change LENGTH and the size of RESULT if you change this!
.orig x4000
    .fill 1
    .fill 3
    .fill 7
    .fill 15
.end

;;  Your answer should be stored here!
;;  You should block (LENGTH - 1) spaces in memory to save your answer.
.orig x4100
    .blkw 3
.end