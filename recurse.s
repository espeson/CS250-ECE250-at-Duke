# $a0 -> n
main:
    subu $sp, $sp, 4
    sw $ra, 0($sp)


    li $v0, 5
    syscall

    add $a0,$v0,$0
    jal _function


    add $a0, $v0, $0 
    li $v0, 1
    syscall
    li $v0, 11     
    li $a0, 0x0A   
    syscall

    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra


_function: 
    subu $sp, $sp, 4
    sw $ra, 0($sp)
    bne $a0, $0, _GeneralCase 
    beq $a0, $0, _BaseCase

_BaseCase: 
    li $v0, 2

    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    
_GeneralCase:
    add $t2, $a0, $a0 
    subu $t2, $t2, 1 
    
    subu    $a0, $a0, 1
   
    subu $sp, $sp, 4 
    sw $t2, 0($sp)
    
    jal _function

    lw $t2, 0($sp)
    addi $sp, $sp, 4

    add $v0, $t2, $v0 

    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra


    


