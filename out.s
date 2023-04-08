.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

main:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -416
  lw $t0, -4($fp)
  li $t0, 14
  lw $t1, -8($fp)
  move $t1, $t0
  lw $t2, -12($fp)
  move $t2, $t1
  lw $t3, -16($fp)
  li $t3, 20
  lw $t4, -20($fp)
  mul $t4, $t2, $t3
  lw $t5, -24($fp)
  li $t5, 8
  lw $t6, -28($fp)
  add $t6, $t4, $t5
  lw $t7, -32($fp)
  move $t7, $t6
  lw $s0, -36($fp)
  move $s0, $t1
  lw $s1, -40($fp)
  move $s1, $t7
  lw $s2, -44($fp)
  move $s2, $t1
  lw $s3, -48($fp)
  mul $s3, $s1, $s2
  lw $s4, -52($fp)
  add $s4, $s0, $s3
  lw $s5, -56($fp)
  li $s5, 100
  lw $s6, -60($fp)
  sub $s6, $s4, $s5
  lw $s7, -64($fp)
  move $s7, $s6
  lw $t8, -68($fp)
  move $t8, $t1
  lw $t9, -72($fp)
  li $t9, 1
  sw $t0, -4($fp)
  sw $t1, -8($fp)
  sw $t2, -12($fp)
  sw $t3, -16($fp)
  sw $t4, -20($fp)
  sw $t5, -24($fp)
  sw $t6, -28($fp)
  sw $s0, -36($fp)
  sw $s1, -40($fp)
  sw $s2, -44($fp)
  sw $s3, -48($fp)
  sw $s4, -52($fp)
  sw $s5, -56($fp)
  sw $s6, -60($fp)
  sw $s7, -64($fp)
  lw $t0, -76($fp)
  mul $t0, $t8, $t9
  lw $t1, -80($fp)
  move $t1, $t7
  lw $t2, -84($fp)
  li $t2, 2
  lw $t3, -88($fp)
  mul $t3, $t1, $t2
  lw $t4, -92($fp)
  add $t4, $t0, $t3
  lw $t5, -96($fp)
  lw $t6, -64($fp)
  move $t5, $t6
  lw $s0, -100($fp)
  li $s0, 3
  lw $s1, -104($fp)
  mul $s1, $t5, $s0
  lw $s2, -108($fp)
  add $s2, $t4, $s1
  lw $s3, -112($fp)
  lw $s4, -8($fp)
  move $s3, $s4
  lw $s5, -116($fp)
  move $s5, $t7
  lw $s6, -120($fp)
  mul $s6, $s3, $s5
  lw $s7, -124($fp)
  move $s7, $t6
  sw $t0, -76($fp)
  sw $t1, -80($fp)
  sw $t2, -84($fp)
  sw $t3, -88($fp)
  sw $t4, -92($fp)
  sw $t5, -96($fp)
  sw $t6, -64($fp)
  sw $s0, -100($fp)
  sw $s1, -104($fp)
  sw $s3, -112($fp)
  sw $s5, -116($fp)
  sw $t8, -68($fp)
  sw $t9, -72($fp)
  lw $t0, -128($fp)
  div $s6, $s7
  mflo $t0
  lw $t1, -132($fp)
  add $t1, $s2, $t0
  lw $t2, -136($fp)
  move $t2, $t1
  lw $t3, -140($fp)
  move $t3, $s4
  lw $t4, -144($fp)
  move $t4, $t7
  lw $t5, -148($fp)
  move $t5, $t7
  lw $t6, -152($fp)
  lw $s0, -64($fp)
  move $t6, $s0
  lw $s1, -156($fp)
  add $s1, $t5, $t6
  lw $s3, -160($fp)
  move $s3, $t2
  lw $s5, -164($fp)
  sub $s5, $s1, $s3
  lw $t8, -168($fp)
  div $t4, $s5
  mflo $t8
  lw $t9, -172($fp)
  move $t9, $t2
  sw $t0, -128($fp)
  sw $t1, -132($fp)
  sw $t2, -136($fp)
  sw $t3, -140($fp)
  sw $t4, -144($fp)
  sw $t5, -148($fp)
  sw $t6, -152($fp)
  sw $t7, -32($fp)
  sw $s1, -156($fp)
  sw $s2, -108($fp)
  sw $s3, -160($fp)
  sw $s4, -8($fp)
  sw $s5, -164($fp)
  sw $s6, -120($fp)
  sw $s7, -124($fp)
  lw $t0, -176($fp)
  mul $t0, $t8, $t9
  lw $t1, -180($fp)
  li $t1, 1
  lw $t2, -184($fp)
  move $t2, $s0
  lw $t3, -188($fp)
  li $t3, 2
  lw $t4, -192($fp)
  div $t2, $t3
  mflo $t4
  lw $t5, -196($fp)
  add $t5, $t1, $t4
  lw $t6, -200($fp)
  mul $t6, $t0, $t5
  lw $t7, -140($fp)
  lw $s1, -204($fp)
  add $s1, $t7, $t6
  lw $s2, -208($fp)
  move $s2, $s1
  lw $s3, -212($fp)
  li $s3, 90
  lw $s4, -216($fp)
  move $s4, $s3
  lw $s5, -220($fp)
  move $s5, $s4
  lw $s6, -224($fp)
  lw $s7, -8($fp)
  move $s6, $s7
  sw $t0, -176($fp)
  sw $t1, -180($fp)
  sw $t2, -184($fp)
  sw $t3, -188($fp)
  sw $t4, -192($fp)
  sw $t5, -196($fp)
  sw $t6, -200($fp)
  sw $t7, -140($fp)
  sw $s0, -64($fp)
  sw $s1, -204($fp)
  sw $s2, -208($fp)
  sw $s3, -212($fp)
  sw $s4, -216($fp)
  sw $s7, -8($fp)
  sw $t8, -168($fp)
  sw $t9, -172($fp)
  lw $t0, -228($fp)
  mul $t0, $s5, $s6
  lw $t1, -232($fp)
  move $t1, $t0
  lw $t2, -236($fp)
  move $t2, $t1
  lw $t3, -240($fp)
  li $t3, 19
  lw $t4, -244($fp)
  add $t4, $t2, $t3
  lw $t5, -248($fp)
  move $t5, $t4
  lw $t6, -252($fp)
  move $t6, $t5
  lw $t7, -256($fp)
  li $t7, 50
  lw $s0, -260($fp)
  sub $s0, $t6, $t7
  lw $s1, -264($fp)
  move $s1, $s0
  lw $s2, -268($fp)
  move $s2, $s1
  lw $s3, -272($fp)
  move $s3, $s1
  lw $s4, -276($fp)
  add $s4, $s2, $s3
  lw $s7, -280($fp)
  move $s7, $s4
  lw $t8, -284($fp)
  move $t8, $s7
  lw $t9, -288($fp)
  move $t9, $t8
  sw $t0, -228($fp)
  sw $t1, -232($fp)
  sw $t2, -236($fp)
  sw $t3, -240($fp)
  sw $t4, -244($fp)
  sw $t5, -248($fp)
  sw $t6, -252($fp)
  sw $t7, -256($fp)
  sw $s0, -260($fp)
  sw $s1, -264($fp)
  sw $s2, -268($fp)
  sw $s3, -272($fp)
  sw $s4, -276($fp)
  sw $s5, -220($fp)
  sw $s6, -224($fp)
  sw $s7, -280($fp)
  sw $t8, -284($fp)
  sw $t9, -288($fp)
  lw $t0, -292($fp)
  lw $t1, -32($fp)
  move $t0, $t1
  sw $t0, -292($fp)
  sw $t1, -32($fp)
  lw $t0, -292($fp)
  move $a0, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $t0, -292($fp)
  lw $t0, -296($fp)
  lw $t1, -136($fp)
  move $t0, $t1
  sw $t0, -296($fp)
  sw $t1, -136($fp)
  lw $t0, -296($fp)
  move $a0, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $t0, -296($fp)
  lw $t0, -300($fp)
  lw $t1, -216($fp)
  move $t0, $t1
  sw $t0, -300($fp)
  sw $t1, -216($fp)
  lw $t0, -300($fp)
  move $a0, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $t0, -300($fp)
  lw $t0, -304($fp)
  lw $t1, -248($fp)
  move $t0, $t1
  sw $t0, -304($fp)
  sw $t1, -248($fp)
  lw $t0, -304($fp)
  move $a0, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $t0, -304($fp)
  lw $t0, -308($fp)
  lw $t1, -32($fp)
  move $t0, $t1
  lw $t2, -312($fp)
  lw $t3, -280($fp)
  move $t2, $t3
  lw $t4, -316($fp)
  lw $t5, -288($fp)
  move $t4, $t5
  lw $t6, -320($fp)
  mul $t6, $t2, $t4
  lw $t7, -324($fp)
  add $t7, $t0, $t6
  lw $s0, -328($fp)
  lw $s1, -264($fp)
  move $s0, $s1
  lw $s2, -332($fp)
  lw $s3, -248($fp)
  move $s2, $s3
  lw $s4, -336($fp)
  lw $s5, -232($fp)
  move $s4, $s5
  lw $s6, -340($fp)
  sub $s6, $s2, $s4
  lw $s7, -344($fp)
  mul $s7, $s0, $s6
  lw $t8, -348($fp)
  lw $t9, -216($fp)
  move $t8, $t9
  sw $t0, -308($fp)
  sw $t1, -32($fp)
  sw $t2, -312($fp)
  sw $t3, -280($fp)
  sw $t4, -316($fp)
  sw $t5, -288($fp)
  sw $t6, -320($fp)
  sw $s0, -328($fp)
  sw $s1, -264($fp)
  sw $s2, -332($fp)
  sw $s3, -248($fp)
  sw $s4, -336($fp)
  sw $s5, -232($fp)
  sw $s6, -340($fp)
  sw $t9, -216($fp)
  lw $t0, -352($fp)
  lw $t1, -208($fp)
  move $t0, $t1
  lw $t2, -356($fp)
  add $t2, $t8, $t0
  lw $t3, -360($fp)
  div $s7, $t2
  mflo $t3
  lw $t4, -364($fp)
  sub $t4, $t7, $t3
  lw $t5, -368($fp)
  lw $t6, -136($fp)
  move $t5, $t6
  lw $s0, -372($fp)
  lw $s1, -64($fp)
  move $s0, $s1
  lw $s2, -376($fp)
  mul $s2, $t5, $s0
  lw $s3, -380($fp)
  add $s3, $t4, $s2
  lw $s4, -288($fp)
  move $s4, $s3
  lw $s5, -384($fp)
  move $s5, $s4
  lw $s6, -388($fp)
  lw $t9, -280($fp)
  move $s6, $t9
  sw $t0, -352($fp)
  sw $t1, -208($fp)
  sw $t2, -356($fp)
  sw $t3, -360($fp)
  sw $t4, -364($fp)
  sw $t5, -368($fp)
  sw $t6, -136($fp)
  sw $t7, -324($fp)
  sw $s0, -372($fp)
  sw $s1, -64($fp)
  sw $s2, -376($fp)
  sw $s3, -380($fp)
  sw $s4, -288($fp)
  sw $s7, -344($fp)
  sw $t8, -348($fp)
  sw $t9, -280($fp)
  lw $t0, -392($fp)
  add $t0, $s5, $s6
  lw $t1, -396($fp)
  lw $t2, -264($fp)
  move $t1, $t2
  lw $t3, -400($fp)
  add $t3, $t0, $t1
  lw $t4, -404($fp)
  lw $t5, -248($fp)
  move $t4, $t5
  lw $t6, -408($fp)
  add $t6, $t3, $t4
  lw $t7, -8($fp)
  move $t7, $t6
  lw $s0, -412($fp)
  move $s0, $t7
  sw $t0, -392($fp)
  sw $t1, -396($fp)
  sw $t2, -264($fp)
  sw $t3, -400($fp)
  sw $t4, -404($fp)
  sw $t5, -248($fp)
  sw $t6, -408($fp)
  sw $t7, -8($fp)
  sw $s0, -412($fp)
  sw $s5, -384($fp)
  sw $s6, -388($fp)
  lw $t0, -412($fp)
  move $a0, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $t0, -412($fp)
  lw $t0, -416($fp)
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
  sw $t0, -416($fp)
