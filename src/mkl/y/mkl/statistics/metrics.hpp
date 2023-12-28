//! \file

#ifndef Y_MKL_Stats_Metrics_Included
#define Y_MKL_Stats_Metrics_Included 1

#include "y/mkl/tao/seq/level1.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Statistics
        {
            template <typename T,typename ITERATOR> inline
            T Average(ITERATOR it, size_t n, Antelope::Add<T> &xadd)
            {
                switch(n)
                {
                    case 0: return T(0);
                    case 1: return T(*it);
                    default:
                        break;
                }
                const T den = n;
                xadd.free();
                while(n-- > 0)
                    xadd << *(it++);
                return xadd.sum()/den;
            }

            template <typename T,typename SEQUENCE> inline
            T Average(SEQUENCE &seq,Antelope::Add<T> &xadd)
            {
                return Average(seq.begin(),seq.size(),xadd);
            }
        }
    }
}

#endif

