#ifndef EON_MACROS_H
#define EON_MACROS_H

#define MAX(a, b)\
    a > b ? a : b

#define MIN(a, b)\
    a < b ? a : b

#define KiB(n) ((uint64_t)(n) << 10)

#define MiB(n) ((uint64_t)(n) << 20)

#define GiB(n) ((uint64_t)(n) << 30)

#define ALIGN_UP_POW2(n, p)\
    (((uint64_t)(n) + ((uint64_t)(p) - 1)) & (~((uint64_t)(p) - 1)))

#ifndef ARENA_ALIGNMENT
    #if _STDC_VERSION_ >= 201112L
        #define ARENA_ALIGNMENT _Alignof(max_align_t)
    #else
        #define ARENA_ALIGNMENT (2 * sizeof(void *))
    #endif
#endif

#endif