#include "y/counting/c/comb.h"
#include <string.h>

void  yack_comb_init(struct yack_comb *param, const size_t n, const size_t k)
{
    //static const size_t sz0 = 0;
    //static const size_t szm = ~sz0;
    //size_t i;
    assert(param);
    assert(n>0);
    assert(k<=n);

    param->n   = n;
    param->k   = k;
    param->nmk = n-k;

#if 0
    uint64_t num = 1;
    {
        const size_t np1 = n+1;
        uint64_t     den = 1;

        for(i=1;i<=k;++i)
        {
            num *= (np1-i);
            den *= i;
            yack_simplify64(&num,&den);
        }
        assert(1==den);
    }

    if(num>szm)
    {
        memset(param,0,sizeof(struct yack_comb));
        return -1;
    }
    else
    {
        param->count = (size_t)num;
        return 0;
    }
#endif
    
}


void yack_comb_boot(const struct yack_comb *param, size_t comb[])
{
    size_t i;
    assert(NULL!=param);
    assert(!(NULL==comb&&param->k>0));
    for(i=param->k;i>0;--i) comb[i] = i;
}

#include <stdio.h>

int yack_comb_next(const struct yack_comb *param, size_t *comb)
{
    assert(NULL!=param);
    assert(NULL!=comb);

    {
        const size_t k   = param->k;
        const size_t nmk = param->nmk;
        size_t       i   = k;
        ++comb[i];
        while( comb[i]>nmk+i )
        {
            if(--i<=0) break;
            ++comb[i];
        }
        
        if(i<=0) return 0;
        
        {
            size_t j=i;
            for(++i;i<=k;++i,++j)
            {
                comb[i] = comb[j]+1;
            }
        }
        
        return 1;
    }
}
