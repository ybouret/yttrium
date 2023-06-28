
#include "y/memory/out-of-reach.h"
#include <string.h>

void *Y_OutOfReach_Addr(void *addr)
{
    return addr;
}

void *Y_OutOfReach_Zero(void *addr, const size_t size)
{
    assert(Y_Good(addr,size));
    memset(addr,0,size);
    return addr;
}

void *Y_OutOfReacMove(void *target, const void *source, const size_t size)
{

    assert(Y_Good(target,size));
    assert(Y_Good(source,size));
    memmove(target,source,size);
    return target;
}

void *Y_OutOfReacSwap(void *target, void *source, const size_t size)
{
    uint8_t *p = (uint8_t *)target;
    uint8_t *q = (uint8_t *)source;
    size_t   i;

    assert(Y_Good(target,size));
    assert(Y_Good(source,size));

    for(i=0;i<size;++i) {
        uint8_t t = p[i];
        p[i]      = q[i];
        q[i]      = t;
    }

    return target;
}

void *Y_OutOfReacMov0(void *target, void *source, const size_t size)
{
    uint8_t *p = (uint8_t *)target;
    uint8_t *q = (uint8_t *)source;
    size_t   i;

    assert(Y_Good(target,size));
    assert(Y_Good(source,size));

    for(i=0;i<size;++i) {
        p[i]      = q[i];
        q[i]      = 0;
    }

    return target;
}

int   Y_OutOfReach_Is0(const void *addr, const size_t size)
{
    const uint8_t *p = (const uint8_t *)addr;
    size_t i;

    assert(Y_Good(addr,size));

    for(i=0;i<size;++i)
    {
        if( p[i] ) return 0;
    }

    return 1;
}

ptrdiff_t Y_OutOfReach_Diff(const void *a, const void *b)
{
    const uint8_t *B=(const uint8_t *)b;
    const uint8_t *A=(const uint8_t *)a;
    return B-A;
}
