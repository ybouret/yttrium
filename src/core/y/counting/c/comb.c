#include "y/counting/c/comb.h"
#include <string.h>

void  Y_Comb_Init(struct Y_Comb *param, const size_t n, const size_t k)
{
    assert(NULL!=param);
    assert(n>0);
    assert(k<=n);

    param->n   = n;
    param->k   = k;
    param->nmk = n-k;

    
}


void Y_Comb_Boot(const struct Y_Comb *param, size_t comb[])
{
    size_t i;
    assert(NULL!=param);
    assert(!(NULL==comb&&param->k>0));
    for(i=param->k;i>0;--i) comb[i] = i;
}

#include <stdio.h>

int Y_Comb_Next(const struct Y_Comb *param, size_t *comb)
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
