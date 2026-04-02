lea $a1, EXP                           
lw $a1, 0($a1)
addi $t1, $a1, 0
halt

EXP:    .fill 8