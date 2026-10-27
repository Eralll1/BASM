# BASM (bad ASM)

## Binary and Syntax

BASM has constant instruction length and the following stucture: <br>
opcode      flags       arg1        arg2        arg3 <br>
uint8_t     uint8_t     int32_t     int32_t     int32_t <br>

flags have the following structure: <br>
    MSB <br>
     | 1-2 bit > Unused     <br>
     | 3-4 bit > arg1 flags <br>
     | 5-6 bit > arg2 flags <br>
     | 7-8 bit > arg3 flags <br>
    LSB

Flag values are the following:


    > 01b        %ax      - register 

    > 10b        &ax      - memory (address is register value)

    > 11b        P        - stack used
                           if it is used as ARG1 or ARG2 stacked is poped
                           else top of the stack (nondestructive reading) is passed
                           the resualt is pushed to the stack 

    > 00b       number   - it is used as constant
                $label   - position of the label is used
                           $label is replaced in compile-time so its value is used as constant 
                "a"      - char code is substituted by compiler
                "/n"     - use "/" (NOT "\" !!!) for escape seq


All three arguments are passed and the resualt (return of `__spu__$NAME`) is stored in ARG3

## Arithmetic

There are two types of values that are indistingiushable: integers(WH) and fixed-point (FP). 
Therefore all type conversions have to be impicit,
to convert them there are TO_FP and TO_WH instructions.
SQRT instruction only takes FP values as input

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

## Important

No jmp instruction can jump to 0 as to prevent errors where JMP instruction has no arguments and 0 default value

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

