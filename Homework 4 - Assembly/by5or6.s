# t0 is n
# t1 is i
main:
        li $t0, 0   # initialize count=0
        li $t1, 1  # $t1 is a counter i

        li $v0,5 # receive input 'n'
        syscall

        move $t0, $v0 # $t0 is N

        lw $t2, divisor5 
        lw $t3, divisor6
        
        j _division

_division:
        div $t1,$t2
        mfhi $t4 # $t4 is the remainer of i/5
        beq $t4, $0, _print # if remainer is 0: ready to print

        div $t1, $t3
        mfhi $t5  # $t5 is the remainer of i/6
        beq $t5, $0, _print # if remainer is 0: ready to print

        addi $t1,$t1,1 # count++

        j _division # jump to the first line of the loop

_print:
        # print the int we want
        move $a0,$t1
        li $v0,1
        syscall
        
        # print "\n"
        li $v0, 11     
        li $a0, 0x0A   
        syscall

        # check if we print enough int or not
        addi $t0, $t0, -1 # t0 is N
        addi $t1,$t1,1

        # if it is enough , jump to _end, otherwise go back _division

        bne $t0,$0, _division
        
        # return 0
        li $v0,0
        jr $ra
.data
        count: .word 0
        divisor6:   .word 6
        divisor5:   .word 5

        