movi $1, beginning
   movi $2, end
   jeq $1, $2, done
loop:
   slti $7, $2, $1
   jeq $7, $0, done
   lw $3, 0($1)
   lw $4, -1($2)
   sw $3, -1($2)
   sw $4, 0($1)
   addi $1, $1, 1
   addi $2, $2, -1
   j loop
done:
   halt
beginning:
   .fill 3
   .fill 1
   .fill 0
   .fill 9
   .fill 5
   .fill 7
   .fill 1
   .fill 4
end: