#include "y/sort/heap.hpp"
#include <cstring>

namespace Yttrium
{
#if 0
    void HeapSort:: Call(void        *p,
                         const size_t n,
                         const size_t itsz,
                         void        *temp,
                         int        (*proc)(const void *,const void *,void*),
                         void        *args) noexcept
    {
        assert(Good(p,n));
        assert(itsz>0);
        assert(0!=temp);
        assert(0!=proc);

        if(n<2) return;
        size_t   l   =(n >> 1)+1;
        size_t   ir  = n;
        uint8_t *rra = static_cast<uint8_t*>(temp);
        uint8_t *ra1 = static_cast<uint8_t*>(p);
        uint8_t *ra  = ra1-itsz;


        while(true)
        {
            if (l>1)
            {
                // rra=ra[--l];
                memcpy(rra,&ra[--l*itsz],itsz);

            }
            else
            {
                //rra=ra[ir];
                //ra[ir]=ra[1];
                uint8_t *r = &ra[ir*itsz];
                for(size_t k=0;k<itsz;++k)
                {
                    rra[k] = r[k];
                    r[k]   = ra1[k];
                }

                if(--ir == 1)
                {
                    //ra[1]=rra;
                    memcpy(ra1,rra,itsz);
                    break;
                }
            }

            size_t    i   = l;
            size_t    j   = l+l;
            uint8_t * r_i = &ra[i*itsz];

            while (j <= ir)
            {
                uint8_t *r_j = &ra[j*itsz];
                {
                    uint8_t *r_k = r_j + itsz;
                    if( (j<ir) && proc(r_j,r_k,args)<0 )
                    {
                        ++j;
                        r_j = r_k;
                    }
                }

                if( proc(rra,r_j,args) < 0 )
                {
                    //ra[i]=ra[j];
                    memcpy(r_i,r_j,itsz);
                    i   = j;
                    r_i = r_j;
                    j <<= 1;
                }
                else
                    break;
            }
            //ra[i]=rra;
            memcpy(r_i,rra,itsz);
        }

    }
#endif
    

}
