
#include "y/chem/algebraic.hpp"

namespace Yttrium
{

    namespace Chemical
    {
        
        Algebraic:: Weight:: ~Weight() noexcept
        {
        }
        
        Algebraic:: Weight:: Weight(const Readable<const apz> &w,
                                    const Matrix<int>         &Nu) :
        Coefficients(w.size()),
        stoi(Nu.cols),
        nEqs(0),
        next(0),
        prev(0)
        {
            assert(w.size()==size());
            assert(w.size()==Nu.rows);
            
            // fetch weights
            for(size_t i=w.size();i>0;--i)
            {
                if( 0 != ( (*this)[i] = w[i].cast<int>("reaction weight") ) )
                {
                    ++Coerce(nEqs);
                }
            }

            // compute stoi
            for(size_t j=Nu.cols;j>0;--j)
            {
                int sum = 0;
                for(size_t i=Nu.rows;i>0;--i)
                {
                    sum += Nu[i][j] * (*this) [i];
                }
                Coerce(stoi[j]) = sum;
            }
            
        }

    }

}


