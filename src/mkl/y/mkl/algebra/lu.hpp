//! \file

#ifndef Y_MKL_LU_Included
#define Y_MKL_LU_Included 1

#include "y/container/matrix.hpp"
#include "y/mkl/antelope/mul.hpp"
#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //
        //! LU decomposition/solving by Crout's algorithm
        //
        //
        //______________________________________________________________________
        template <typename T>
        class LU
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit LU() noexcept;         //!< start empty
            explicit LU(const size_t nmin); //!< start with capacity
            virtual ~LU() noexcept;         //!< cleanup

            //______________________________________________________________________
            //
            //
            // Methods
            //
            //______________________________________________________________________
            void ensure(const size_t nmin);                   //!< ensure memory for nmin x nmin matrices
            bool build(Matrix<T> &a);                         //!< try to decompose matrix
            void solve(const Matrix<T>  &a, Writable<T> &b);  //!< in-place solving
            void solve(const Matrix<T>  &a, Matrix<T>   &b);  //!< in-place solve all columns
            void invert(const Matrix<T> &a, Matrix<T>   &b);  //!< b=a^-1, a is decomposed
            T    determinant(const Matrix<T> &a);             //!< determinant of pre-built matrix
            void adjoint(Matrix<T> &adj, const Matrix<T> &a); //!< adjoint

            //! generic lhs = a^-1 * rhs, post build
            template<typename LHS, typename RHS> inline
            void solve(LHS &lhs, const Matrix<T> &a, RHS &rhs)
            {
                assert(0!=code);
                const size_t n   = a.rows;
                assert(n>0);
                assert(rhs.size()>=n);
                assert(lhs.size()>=n);
                Writable<T> &aux = getAux(); assert(aux.size()>=n);
                for(size_t i=n;i>0;--i) aux[i] = rhs[i];
                solve(a,aux);
                for(size_t i=n;i>0;--i) lhs[i] = aux[i];
            }

            Antelope::Add<T> & xadd() noexcept; //!< if code was activated
            Antelope::Mul<T> & xmul() noexcept; //!< if code was activated




        private:
            Y_DISABLE_COPY_AND_ASSIGN(LU);
            class Code;
            Code *code;
            Writable<T> & getAux() noexcept; //!< if code was activated

        };

    }

}

#endif
