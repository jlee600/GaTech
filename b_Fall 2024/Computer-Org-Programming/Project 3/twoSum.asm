;;=============================================================
;; CS 2110 - Fall 2024
;; Project 3 - Two Sum
;;=============================================================
;; Name: Jinseo Lee
;;=============================================================

;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  n = LENGTH;
;;  for (i = 0; i < n; i++) {
;;      for (j = i + 1; j < n; j++) {
;;          if (array[i] + array[j] == target) {
;;              ANSWER = [i, j];
;;              return;
;;          }
;;      }
;;  }

.orig x3000
; R0 = mem[LENGTH]
LD R0, LENGTH
; R1 = mem[TARGET]
LD R1, TARGET
; R2 = startArr = mem[ARRAY]
LD R2, ARRAY
; R3 = index = i
AND R3, R3, #0

i_LOOP
; R4 = -length
NOT R4, R0
ADD R4, R4, #1
; index - length
ADD R4, R3, R4
BRzp FINISH

; R4 = index + 1 = j
ADD R4, R3, #1

j_LOOP
; index + 1 - length
NOT R5, R0
ADD R5, R5, #1
ADD R5, R4, R5
BRzp i_NEXT

; R5 = array[startArr + i]
ADD R5, R2, R3
LDR R5, R5, #0
; R6 = array[startArr + j]
ADD R6, R2, R4
LDR R6, R6, #0
; R6 = R5 + R6 - target
NOT R7, R1
ADD R7, R7, #1
ADD R6, R5, R6
ADD R6, R6, R7
; if R6 == 0
BRz FINISH

j_NEXT
ADD R4, R4, #1
BR j_LOOP

i_NEXT
ADD R3, R3, #1
BR i_LOOP

FINISH
LEA R5, ANSWER
STR R3, R5, #0
STR R4, R5, #1
HALT
    
;;  You can change this value to debug!    
LENGTH .fill 5    
TARGET .fill 9
    
;;  Do not change these values!
ARRAY  .fill x4000
ANSWER .blkw 2
.end


;;  Array Values - You can change these to debug!
;;  If you do, remember to change LENGTH accordingly!
.orig x4000
    .fill 2
    .fill 1
    .fill 6
    .fill 3
    .fill 4
.end