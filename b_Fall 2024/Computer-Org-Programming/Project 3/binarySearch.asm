;;=============================================================
;; CS 2110 - Fall 2024
;; Project 3 - Binary Search (With Right Shift)
;;=============================================================
;; Name: Jinseo Lee
;;=============================================================

;;  PLEASE READ THE FULL PDF BEFORE STARTING
;;  Implement Right Shift before Binary Search, as Binary Search uses Right Shift

;;  Right Shift Suggested Pseudocode (see PDF for explanation)
;;
;;  val = R0;
;;  amt = R1;
;;  result = 0;
;;  while (amt < 16) {
;;      result = result + result;
;;      if (val < 0) { 
;;          result = result++;
;;      }
;;      val = val + val;
;;      amt++;
;;  }
;;  R0 = result;

.orig x3100
RIGHTSHIFT
    AND R2, R0, 0   ; result = 0
    
    WHILE
    ; R6 = amt - 16
    ADD R6, R1, -15
    ADD R6, R6, -1
    BRzp EWHILE
    
    ADD R2, R2, R2
    ADD R0, R0, 0   ; cc val
    BRzp OUTER
    ADD R2, R2, 1
    
    OUTER
    ADD R0, R0, R0
    ADD R1, R1, 1
    BR WHILE
    
    EWHILE
    ADD R0, R2, 0   ; R0 = R2
    RET
.end

;;  Binary Search Suggested Pseudocode (see PDF for explanation)
;;
;;  low = 0;
;;  high = LENGTH - 1;
;;  while (low <= high) {
;;      sum = low + high;
;;      mid = RIGHTSHIFT(sum, 1);
;;      if (arr[mid] == TARGET) {
;;          mem[RESULT] = mid;
;;          return;
;;      } else if (arr[mid] < TARGET) {
;;          low = mid + 1;
;;      } else {
;;          high = mid - 1;
;;      }
;;  }
;;  mem[RESULT] = -1;

.orig x3000
; R0 must be sum
; R1 must be amt
; mid stored in R0
    AND R3, R3, 0   ; R3 = low = 0
    LD R4, LENGTH
    ADD R4, R4, -1  ; R4 = high
    
    _WHILE
    NOT R5, R4  
    ADD R5, R5, 1; R5 = -R4
    ADD R5, R3, R5  ; R5 = low - high
    BRp E_WHILE
    
    ADD R0, R3, R4  ; R0 = sum = low + high
    AND R1, R1, 0
    ADD R1, R1, 1   ; R1 = amt = 1
    
    ; upto here, R0 = sum, R1 = amt, R3 = low, R4 = high, R5 = low - high
    JSR RIGHTSHIFT ; R0 = mid
    
    LD R1, ARRAY
    ADD R1, R1, R0
    LDR R1, R1, 0   ; R1 = arr[mid]
    LD R2, TARGET
    NOT R2, R2
    ADD R2, R2, 1   ; R2 = -target
    
    ADD R2, R1, R2  ; R2 = arr[mid] - target
    BRn ELIF
    BRp ELSE
    
    LEA R2, RESULT  ; R2 = result
    STR R0, R2, #0 ; mem[result] = R0
    BR TERMINATE
    
    ELIF
    ADD R3, R0, 1   ; R3 = low = mid + 1
    BR _WHILE
    
    ELSE
    ADD R4, R0, -1  ; R4 = high = mid - 1
    BR _WHILE
    
    E_WHILE
    LEA R0, RESULT   ; R0 = result
    AND R1, R1, 0
    ADD R1, R1, -1  ; R1 = -1
    STR R1, R0, 0   ; mem[result] = -1
    
    TERMINATE
    HALT

;;  You can change these to debug!
LENGTH .fill 6
TARGET .fill -3

;;  Do not change these values
ARRAY  .fill x4000
RESULT .blkw 1
STACK  .fill x6000
.end

;;  ARRAY VALUES - You can change these to debug!
;;  Make sure they are sorted if you change values!
;;  If you do change, make sure you change LENGTH/TARGET accordingly!
.orig x4000
    .fill -5
    .fill -3
    .fill 1
    .fill 4
    .fill 5
    .fill 6
.end
    