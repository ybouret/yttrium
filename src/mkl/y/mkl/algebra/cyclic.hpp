
//! \file

#ifndef Y_MKL_Algebra_Cyclic_Included
#define Y_MKL_Algebra_Cyclic_Included 1

#include "y/mkl/algebra/tridiag.hpp"

namespace Yttrium
{
    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //
        //! Cyclic (a,b,c)
        //
        //
        //__________________________________________________________________
        template <typename T>
        class Cyclic : public TriDiag<T>
        {
        public:
            using TriDiag<T>::size;
            using TriDiag<T>::a;
            using TriDiag<T>::b;
            using TriDiag<T>::c;

            explicit Cyclic(const size_t n); //!< n >= 3
            virtual ~Cyclic() noexcept;


            //! try to solve this * u = r
            bool  solve(Writable<T> &u, const Readable<T> &r);

            //! display as a dense matrix
            inline friend std::ostream & operator<<(std::ostream &os, const Cyclic &self)
            {
                Matrix<T> M(self.size,self.size);
                self.sendTo(M);
                os << M;
                return os;
            }

            //! res = this * rhs
            template <typename RES, typename RHS> inline
            void mul(RES &res, RHS &rhs) const
            {
                const size_t n = size; assert(n>=2);

                // first row
                {
                    const T u = b[1] * rhs[1];
                    const T v = c[1] * rhs[2];
                    const T w = beta * rhs[n];
                    res[1]    = Antelope::Sum3<T>::Of(u,v,w);
                }

                // core
                const size_t nm=n-1;
                for(size_t i=nm,im=i-1,ip=i+1;i>1;--i,--im,--ip)
                {
                    const T A = a[i] * rhs[im];
                    const T B = b[i] * rhs[i];
                    const T C = c[i] * rhs[ip];
                    res[i]    = Antelope::Sum3<T>::Of(A,B,C);
                }

                // last row
                {
                    const T u = b[n]  * rhs[n];
                    const T v = a[n]  * rhs[nm];
                    const T w = alpha * rhs[1];
                    res[n]    = Antelope::Sum3<T>::Of(u,v,w);
                }

            }
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cyclic);
            class  Code;
            Code  *code;
        public:
            T &    alpha; //!< (n,1) = c[n]
            T &    beta;  //!< (1,n) = a[1]

            //! return a copy of item, depending on i,j
            T operator()(const size_t i, const size_t j) const;

            //! send to size x size matrix
            template <typename U>
            inline void sendTo(Matrix<U> &M) const
            {
                assert(M.rows==size);
                assert(M.cols==M.rows);
                const Cyclic  &self = *this;
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
