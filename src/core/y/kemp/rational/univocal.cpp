
#include "y/kemp/rational/univocal.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        const char * const Univocal::CallSign = "Kemp::Univocal";

        apq & Univocal:: UpdateGCD(Natural &g, const apq &q)
        {
            const Natural &rhs = q.numer.n;
            if(rhs>0)
            {
                if(g<=0)
                    g = rhs;
                else
                    g = Natural::GCD(g,rhs);
            }
            return Coerce(q);
        }

        const Natural & Univocal:: Dispatch(size_t    & numPos,
                                            size_t    & numNeg,
                                            SignType  & firstSign,
                                            const apq & q) noexcept
        {
            const SignType s = q.numer.s;

            if(__Zero__==firstSign) firstSign = s;

            switch(s)
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


        const Natural & Univocal:: Dispatch(size_t    & numPos,
                                            size_t    & numNeg,
                                            SignType  & firstSign,
                                            const apz & z) noexcept
        {
            const SignType s = z.s;

            if(__Zero__==firstSign) firstSign = s;

            switch(s)
            {
                case Negative:
                    ++numNeg;
                    break;
                case Positive:
                    ++numPos;
                    break;
                case __Zero__: break;
            }
            return z.n;
        }


        void Univocal:: MakeMatrix(Matrix<apz> &a)
        {
            if(a.rows<=0) return;
            const size_t                 n = a.cols; assert(n>0);
            CxxArray<apq,Memory::Dyadic> q(n);
            for(size_t i=a.rows;i>0;--i)
            {
                MatrixRow<apz> &r = a[i];
                for(size_t j=n;j>0;--j) q[j] = r[j];
                MakeRational(q);
                for(size_t j=n;j>0;--j) r[j] = q[j].numer;
            }
        }
    }

}


