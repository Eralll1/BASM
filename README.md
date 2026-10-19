# BASM (bad ASM)

## Binary and Syntax

BASM has constant instruction length and the following stucture:
opcode      flags       arg1        arg2        arg3
uint8_t     uint8_t     uint32_t    uint32_t    uint32_t

flags have the following structure:
    MSB
     | 1-2 bit > Unused
     | 3-4 bit > arg1 flags
     | 5-6 bit > arg2 flags
     | 7-8 bit > arg3 flags
    LSB

Flag values are the following:
    > 01        %ax      - register

    > 10        &ax      - memory (address is register value)

    > 11        P        - stack used
                           if it is used as ARG1 or ARG2 stacked is poped
                           else top of the stack (nondestructive reading) is passed
                           the resualt is pushed to the stack 

    > 00        number   - it is used as constant
                $label   - position of the label is used
                           $label is replaced in compile-time so its value is used as constant 
                "a"      - char code is substituted by compiler
                "/n"     - use "/" (NOT "\" !!!) for escape seq

All three arguments are passed and the resualt (return of `__spu__$NAME`) is stored in ARG3

## Editing

All instructions data is stored in globals/INSTSET file

To add new instructions: 
    > add them with UNIQUE opcode to INSTSET
    > add it's prototype to functions.h
        word `__spu__$NAME`    (word arg1, word arg2, word arg3, Proccesor* proc);
        where $NAME is instruction name in LOWER FUCKING CASE
    > add the function itself to function.c

Most of tweakable values are located in globals/globals.h and globals/ray_glob.h 
Some can be in other globals/*.h files

## Compilation and Usage

### Proccessor

Compile:
> make better 
Run:
> ./better -i INPUT.bin

### Assembler

Compile:
> make asm_main
Run:
> ./asm_main -i INPUT.txt -o OUTPUT.bin


## TODO

1 errors

2 code checks

3 the whole proccessor to struct + functions        +

4 JB(<) JA(>) JE(==) JAE(>=) JBE(<=) JNE(!=)        +

5 processor dump

6 no dup labels                                     +

7 tab at start?                                     +

8 check invalid jmp address                         +

9 # -> ;                                            + both are supported

10 printf before scanf                              +

11 output as char                                   +

12 invalid input crash

13 registers - names                                +

14 prototypes!

15 RAM address in processor struct and const SIZE   +

16 syscalls <- input \ output \ draw                - declined

17 scanf                                            + 

18 print compiler errors                            + (fixed some small issue, when ERR intruction was not considered as an error)

19 parse_instruction too F. big

20 ERR_LINE_TXT  

21 disable JMP to 0 to prevent errors