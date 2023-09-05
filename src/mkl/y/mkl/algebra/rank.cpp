
#include "y/mkl/algebra/rank.hpp"

namespace Yttrium
{
    namespace MKL
    {
        size_t Rank:: Compute(Matrix<apq> &Q)
        {
            const size_t m    = Q.rows; // |rows|
            const size_t n    = Q.cols; // |cols|
            size_t       h    = 1;      // pivot row
            size_t       k    = 1;      // pivot col
            size_t       rank = 0;      // current ranl
            const apq    _0   = 0;      // helper

            //______________________________________________________________________
            //
            // Running on rows and columns
            //______________________________________________________________________
            while(h<=m && k<=n)
            {
                //__________________________________________________________________
                //
                // initialize @row #h the search for pivot @column #k
                //__________________________________________________________________
                Readable<apq> &Qh = Q[h];
                size_t i_max = h;
                apq    q_piv = Qh[k];
                apq    a_max = q_piv.abs();

                //__________________________________________________________________
                //
                // look for pivot in column k
                //__________________________________________________________________
                for(size_t i=h+1;i<=m;++i) {
                    const apq &q_tmp = Q[i][k];
                    const apq  a_tmp = q_tmp.abs();
                    if(a_tmp>a_max) {
                        q_piv = q_tmp;
                        a_max = a_tmp;
                        i_max = i;
                    }
                }

                if(a_max<=0) {
                    ++k; // pass to next column
                }
                else {
                    assert(a_max>0);
                    assert(q_piv!=0);

                    ++rank;                // found a valid rank
                    Q.swapRows(h,i_max);  // at i_max
                    assert(q_piv==Qh[k]);
                    for(size_t i=h+1;i<=m;++i)
                    {
                        assert(Q[h][k]!=0);
                        Writable<apq> &Qi = Q[i];
                        const apq      f  = Qi[k]/q_piv;
                        Q[i][k]     = _0;
                        for(size_t j = k+1; j<=n; ++j)
                        {
                            Qi[j] -= Qh[j] * f;
                        }
                    }
                    ++h; // pass to next line
                    ++k; // pass to next column
                }
            }

            return rank;
        }

    }

}

