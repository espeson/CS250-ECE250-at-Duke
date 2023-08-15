main:
    addi $sp, $sp, -16
    sw $s0, 0($sp)
    sw $ra, 12($sp)
############################################################################################
    # Initialize variables
    li $s0, 0         
    move $t0, $s0     
    move $t2, $s0     
############################################################################################
KeepAddingNode:

    sw $t0, 4($sp)
    sw $t1, 8($sp)


    # Allocate memory for a new node
    li $v0, 9
    li $a0, 72
    syscall         
    move $t1, $v0  

    # Prompt the user for a name
    li $v0, 4
    la $a0, nameprompt
    syscall

    li $v0, 8
    la $a0, NameSpace
    li $a1, 64
    syscall         

    move $a2, $t1 
    
    addi $sp, $sp, -8
    sw $a2, 0($sp)
    sw $ra, 4($sp)
    jal StringCopy 
    lw $a2, 0($sp)
    lw $ra, 4($sp)
    addi $sp, $sp, 8

  # Check if the user is done entering data
    la $a0, DONE
    jal StringCompare
    beq $v0, $zero, PrintLinkedList   # If so, print the nodes and exit

 # Prompt the user for data and calculate the result
    li $v0, 4
    la $a0, APSprompt
    syscall
    li $v0, 6
    syscall 
    mov.s $f4, $f0 

    li $v0, 4
    la $a0, APGUprompt
    syscall
    li $v0, 6
    syscall 
    mov.s $f5, $f0 

    li $v0, 4
    la $a0, ARDprompt
    syscall
    li $v0, 6
    syscall 
    mov.s $f9, $f0 


    jal ResultCalculation 

    s.s $f4, 64($t1) 
    sw $zero, 68($t1) 

     
    # Insert the new node into the linked list
    # First node
    beq $s0, $zero, AddFirstNode # If the list is empty, add the new node as the first node

    
    
    # insert before head: BaskerballCompare(node $t1, head $s0) returns $v0 > 0
    move $a0, $t1 
    move $a1, $s0 
    jal BaskerballCompare
    bgt $v0, $zero, AddNodeBeforeHead
    
    
    # insert after tail: BaskerballCompare(tail $t2, node $t1) returns $v0 > 0
    move $a0, $t2 
    move $a1, $t1 
    jal BaskerballCompare
    bgt $v0, $zero, AddNodeAfterTail

    

    move $t0, $s0 
    lw $t4, 68($s0) 
    j GoThroughTheLinkedList

    lw $t0, 4($sp)
    lw $t1, 8($sp)


    j KeepAddingNode



AddNodeBeforeHead:
    
    move $t4, $s0 # Store current head node in t4 register
    move $s0, $t1 # Update head pointer to new node in s0 register
    sw $t4, 68($s0) # Store the previous head node as the second node in the linked list

    # Load arguments from stack
    lw $t0, 4($sp)
    lw $t1, 8($sp)
    j KeepAddingNode

AddFirstNode:
    move $s0, $t1 
    move $t2, $t1 

    lw $t0, 4($sp)
    lw $t1, 8($sp)
    j KeepAddingNode

AddNodeAfterTail:
    sw $t1, 68($t2) 
    lw $t2, 68($t2) 

    lw $t0, 4($sp)
    lw $t1, 8($sp)
    j KeepAddingNode



GoThroughTheLinkedList:
    move $a0, $t1 
    move $a1, $t4 
    jal BaskerballCompare
    bgt $v0, $zero, InsertNodeBetweenTwoNodes 

    # else: node goes after curr1
    move $t0, $t4 
    lw $t4, 68($t4) 
    j GoThroughTheLinkedList
InsertNodeBetweenTwoNodes:
    sw $t1, 68($t0) 
    sw $t4, 68($t1) 

    lw $t0, 4($sp)
    lw $t1, 8($sp)
    j KeepAddingNode

StringCopy: # copy string from $a0 to $a2
    lb $t5, 0($a0)
    sb $t5, 0($a2)

    beqz $t5, EndOfStringCopy

    addi $a2, $a2, 1
    addi $a0, $a0, 1

    j StringCopy
EndOfStringCopy:
    addi $a2, $a2, -1
    sb $0, 0($a2)
    jr $ra # return to KeepAddingNode

StringCompare:
    lb $t5, 0($a0) 
    lb $t6, 0($a2) 

    beqz $t6, _done

    sub $v0, $t6, $t5
    bnez $v0, EndofStringCompare


    beq $t5, $zero, EndofStringCompare


    addi $a0, $a0, 1
    addi $a2, $a2, 1
    j StringCompare
EndofStringCompare:
    jr $ra # return to KeepAddingNode
_done:
    li $v0, 0
    jr $ra # return to KeepAddingNode: $v0 = 0


ResultCalculation:


############################################################################################



    mtc1 $zero, $f8

    add.s	$f6, $f4, $f9
    sub.s	$f6, $f6, $f5
    add.s   $f4, $f6, $f8 
    jr $ra

############################################################################################












BaskerballCompare: 
    l.s $f1, 64($a0)    # Load the float value of player 1's statistic from offset 64 of the first argument
    l.s $f2, 64($a1)    # Load the float value of player 2's statistic from offset 64 of the second argument
    sub.s $f0, $f1, $f2 # Subtract the value of player 2's statistic from player 1's and store the result in register $f0

    mfc1 $v0, $f0       # Move the float value of $f0 to integer register $v0


############################################################################################
#    bne $v0, $zero, _skip
#    la $a0,

#_skip:
############################################################################################

############################################################################################
    move $a2, $a1
############################################################################################


    beq $v0, $zero, StringCompare 
    jr $ra 


PrintLinkedList:
    li $v0, 4
    la $a0, 0($s0) # print name
    syscall

    li $v0, 4
    la $a0, SPACE
    syscall

    li $v0, 2
    l.s $f12, 64($s0)  
    #syscall


############################################################################################
    cvt.w.s $f0, $f12
    li $v0, 1
    mfc1 $a0, $f0 # print result
    syscall

############################################################################################


    li $v0, 4
    la $a0, NEWLINE
    syscall

    lw $s0, 68($s0) 
    bne $s0, $zero, PrintLinkedList 

EndOfMain:

    lw $s0, 0($sp)
    lw $ra, 12($sp)
    addi $sp, $sp, 16
    jr $ra

.data
    nameprompt: .asciiz "Team name: "
    APSprompt: .asciiz "average points scored: "
    APGUprompt: .asciiz "average points given up: "
    ARDprompt: .asciiz "average rebounding differential:"
############################################################################################ 
    NewNumberprompt: .asciiz "New Number:"
    Debugrprompt: .asciiz "Debuging..."
############################################################################################
    DONE: .asciiz "DONE\n"
    NameSpace: .space 64 # address
    SPACE: .asciiz " "
    NEWLINE: .asciiz "\n"