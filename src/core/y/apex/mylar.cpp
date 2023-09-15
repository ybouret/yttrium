#include "y/apex/mylar.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Apex
    {

        const Natural & Mylar:: Dispatch(size_t    & numPos,
                                         size_t    & numNeg,
                                         SignType  & firstSign,
                                         const apq & q) noexcept
        {
            const SignType s = q.numer.s;

            if(__Zero__==firstSign) firstSign = s;

            switch(q.numer.s)
            {
                case Negative:
                    ++numNeg;
                    break;
                case Positive:
                    ++numPos;
                    break;
                case __Zero__: break;
            }
            return q.denom;
        }

        void Mylar:: MakeUnivocal(Matrix<apz> &a)
        {
            if(a.rows<=0) return;
            const size_t                 n = a.cols; assert(n>0);
            CxxArray<apq,Memory::Dyadic> q(n);
            for(size_t i=a.rows;i>0;--i)
            {
                MatrixRow<apz> &r = a[i];
                for(size_t j=n;j>0;--j) q[j] = r[j];
                Univocal(q);
                for(size_t j=n;j>0;--j) r[j] = q[j].numer;
            }
        }

    }
}
