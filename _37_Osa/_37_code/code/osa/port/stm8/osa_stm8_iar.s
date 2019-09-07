  NAME ATOMPORTASM
  SECTION .text:code

#include "vregs.inc"


  PUBLIC _OS_JumpToTask_ASM
  EXTERN _OS_JumpToTask


        EXTERN  ?b8
        EXTERN  ?b9
        EXTERN  ?b10
        EXTERN  ?b11
        EXTERN  ?b12
        EXTERN  ?b13
        EXTERN  ?b14
        EXTERN  ?b15

_OS_JumpToTask_ASM:


    PUSH ?b8
    PUSH ?b9
    PUSH ?b10
    PUSH ?b11
    PUSH ?b12
    PUSH ?b13
    PUSH ?b14
    PUSH ?b15

    call _OS_JumpToTask

    POP ?b15
    POP ?b14
    POP ?b13
    POP ?b12
    POP ?b11
    POP ?b10
    POP ?b9
    POP ?b8
    ret


    end
