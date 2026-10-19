#include "globals.h"
#include "processor.h"
#include "ray_glob.h"

word __spu__eof     (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__err     (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__empty   (word arg1, word arg2, word arg3, Proccesor* proc);

word __spu__nope    (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__j_alw   (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__j_eq    (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__j_gt    (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__j_lt    (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__j_ge    (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__j_le    (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__j_neq   (word arg1, word arg2, word arg3, Proccesor* proc);

word __spu__call    (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__ret     (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__halt    (word arg1, word arg2, word arg3, Proccesor* proc);

word __spu__move    (word arg1, word arg2, word arg3, Proccesor* proc);

word __spu__add     (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__sub     (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__div     (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__mult    (word arg1, word arg2, word arg3, Proccesor* proc);

word __spu__and     (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__or      (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__xor     (word arg1, word arg2, word arg3, Proccesor* proc);

word __spu__mod     (word arg1, word arg2, word arg3, Proccesor* proc);

word __spu__sqrt    (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__to_fp   (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__to_wh   (word arg1, word arg2, word arg3, Proccesor* proc);

word __spu__inp     (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__pr_wh   (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__pr_ch   (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__pr_fp   (word arg1, word arg2, word arg3, Proccesor* proc);

word __spu__init_screen   (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__draw          (word arg1, word arg2, word arg3, Proccesor* proc);
word __spu__kill_screen   (word arg1, word arg2, word arg3, Proccesor* proc);

