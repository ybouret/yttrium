//! \file

#ifndef Y_MKL_Algebra_TriDiag_Included
#define Y_MKL_Algebra_TriDiag_Included 1

#include "y/container/matrix.hpp"
#include "y/mkl/antelope/sum3.hpp"

namespace Yttrium
{
    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //
        //! Tri-Diagonal Matrix (a,b,c)
        //
        //
        //__________________________________________________________________
        template <typename T>
        class TriDiag
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit TriDiag(const size_t n); //!< setup with size=m
            virtual ~TriDiag() noexcept;      //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            
            //! try to solve this * u = r
            bool  solve(Writable<T> &u, const Writable<T> &r);

            //! try to solve with a foreing diagonal
            bool  solve(Writable<T> &u, const Writable<T> &r, const Readable<T> &B);


            //! res = this * rhs
            template <typename RES, typename RHS> inline
            void mul(RES &res, RHS &rhs) const
            {
                const size_t n = size;
                switch(n)
                {
                    case 0: return;
                    case 1: res[1] = b[1] * rhs[1]; return;
                    default:
                        break;
                }
                res[1] = b[1] * rhs[1] + c[1] * rhs[2];
                const size_t nm=n-1;
                for(size_t i=nm,im=i-1,ip=i+1;i>1;--i,--im,--ip)
                {
                    const T A = a[i] * rhs[im];
                    const T B = b[i] * rhs[i];
                    const T C = c[i] * rhs[ip];
                    res[i] = Antelope::Sum3<T>::Of(A,B,C);
                }
                res[n] = a[n] * rhs[nm] + b[n] * rhs[n];
            }

            inline friend std::ostream & operator<<(std::ostream &os, const TriDiag &self)
            {
                Matrix<T> M(self.size,self.size);
                self.sendTo(M);
                os << M;
                return os;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(TriDiag);
            class Code;
            Code *code;
        public:
            Writable<T>  & a; //!< a[2..size]
            Writable<T>  & b; //!< b[1..size]
            Writable<T>  & c; //!< c[1..size-1]
            const size_t & size;

            //! return a copy of item, depending on i,j
            T operator()(const size_t i, const size_t j) const;

            template <typename U>
            inline void sendTo(Matrix<U> &M) const
            {
                assert(M.rows==size);
                assert(M.cols==M.rows);
                const TriDiag &self = *this;
                const size_t   n    = self.size;
                for(size_t i=n;i>0;--i)
                {
                    Writable<U> &M_i = M[i];
                    for(size_t j=n;j>0;--j) M_i[j] = self(i,j);
                }
            }

        };
    }
}

#endif

