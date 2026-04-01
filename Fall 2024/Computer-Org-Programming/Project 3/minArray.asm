;;=============================================================
;; CS 2110 - Fall 2024
;; Project 3 - Minimum of Array
;;=============================================================
;; Name: Jinseo Lee
;;=============================================================

;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  answer = 2110;
;;  i = 0;
;;  arrLength = LENGTH;
;;  while (arrLength > 0) {
;;      currNum = arr[i];
;;      if (currNum < answer) {
;;          answer = currNum;
;;      }
;;      i++;
;;      arrLength--;
;;  }
;;  mem[mem[ANSWERADDR]] = answer;

.orig x3000
;   R0 = mem[answer]
LD R0, MAX
;   R1 = arrLength = mem[length]
LD R1, LENGTH
;   R2 = index = 0
AND R2, R2, 0
;   R3 = arrStart = mem[array]
LD R3, ARRAY

LOOP
ADD R1, R1, #0
BRnz FINISH

    ; indexAddr = arrStart + index
    ADD R4, R3, R2
    ; currNum = arr[indexAddr]  
    LDR R5, R4, #0
    ; R7 = - answer
    NOT R7, R0
    ADD R7, R7, #1
    ; currNum - answer
    ADD R6, R5, R7
    ; if currNum - answer >= 0
    BRzp INNER
    ; answer = currNum
    ADD R0, R5, #0
    INNER
        ;   index++
        ADD R2, R2, #1
        ;   arrLength--
        ADD R1, R1, #-1
        
    BR LOOP

FINISH
    ;   R1 = mem[ANSEWRADDR] 
    LD R1, ANSWERADDR
    ;   mem[R1] = answer = R0;
    STR R0, R1, #0
    HALT

;; You can change this for debugging!
LENGTH .fill 5

;; Do not change these values!
MAX .fill 2110
ARRAY .fill x4000
ANSWERADDR .fill x4100
.end

;; ARRAY VALUES - You can changes these to debug!
;; If you do change them, make sure you also change length accordingly!
.orig x4000
    .fill 1
    .fill -200
    .fill -4
    .fill -5
    .fill -100
.end

;; Your answer should end up stored here!
.orig x4100
    .blkw 1
.end
