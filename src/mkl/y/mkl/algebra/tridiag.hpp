//! \file

#ifndef Y_MKL_Algebra_TriDiag_Included
#define Y_MKL_Algebra_TriDiag_Included 1

#include "y/container/writable.hpp"
#include "y/mkl/antelope/sum3.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class TriDiag
        {
        public:
            explicit TriDiag(const size_t n);
            virtual ~TriDiag() noexcept;

            size_t size() const noexcept;
            bool  solve(Writable<T> &u, const Writable<T> &r);

            template <typename RES, typename RHS> inline
            void mul(RES &res, RHS &rhs) const
            {
                const size_t n = size();
                switch(n)
                {
                    case 0: return;
                    case 1: res[1] = b[1] * rhs[1]; return;
                    default:
                        break;
                }
                res[1] = b[1] * rhs[1] + c[1] * rhs[2];
                const size_t nm=n-1;
                for(size_t i=nm;i>1;--i)
                {
                    const T A = a[i] * rhs[i-1];
                    const T B = b[i] * rhs[i];
                    const T C = c[i] * rhs[i+1];
                    res[i] = Antelope::Sum3<T>::Of(A,B,C);
                }
                res[n] = a[n] * rhs[nm] + b[n] * rhs[n];
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(TriDiag);
            class Code;
            Code *code;
        public:
            Writable<T> &a;
            Writable<T> &b;
            Writable<T> &c;

            T operator()(const size_t i, const size_t j) const;

        };
    }
}

#endif

