
//! \file

#ifndef Y_MKL_Filter_SavGol_Included
#define Y_MKL_Filter_SavGol_Included 1


#include "y/mkl/filter/resago.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //! Shared Factory of rational matrices
        //
        //______________________________________________________________________
        typedef ArcPtr<ReSaGo::Factory> SavGolFactory;

        //______________________________________________________________________
        //
        //
        //
        //! Factory of (extended) floating point matrices
        //
        //
        //______________________________________________________________________
        template <typename T>
        class SavGol
        {
        public:
            typedef Antelope::Add<T> XAdd;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            //! setup with shared factory of rational matrices
            explicit SavGol(const SavGolFactory &factory);
            virtual ~SavGol() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! get/create matrix from shared factory and convert to T
            const Matrix<T> & fetch(const uint32_t nLeft,
                                    const uint32_t nRight,
                                    const uint32_t degree);

            const Matrix<T> & get(const size_t  n,
                                  const size_t  j,
                                  const size_t  d);


            //! apply filter
            /**
             \param out    [0:num-1] filtered output
             \param idx    [0:num-1] requested orders
             \param num    number of filter
             \param I      data iterator
             \param n      data size
             \param j      data center index
             \param degree polynomial fit order
             */
            template <typename ITERATOR>
            void run(T              out[],
                     const size_t   idx[],
                     const size_t   num,
                     const ITERATOR I,
                     const size_t   n,
                     const size_t   j,
                     const size_t   degree)
            {
                assert(n>0);
                assert(j>0);
                assert(j<=n);
                assert(Good(out,num));
                assert(Good(idx,num));
                assert(degree<n);
                xadd.free();
                const Matrix<T> &filter = get(n,j,degree);
                for(size_t i=0;i<num;++i)
                {
                    const size_t       k = idx[i]; assert(k<filter.rows);
                    const Readable<T> &f = filter[k+1];
                    ITERATOR           it = I;
                    for(size_t l=1;l<=n;++l,++it)
                    {
                        xadd << f[l] * (*it);
                    }
                    out[i] = xadd.sum();
                }
            }

            template <typename ITERATOR>
            T operator()(const ITERATOR I,
                         const size_t   n,
                         const size_t   j,
                         const size_t   degree)
            {
                static const size_t idx[1] = { 0 };
                T                   out[1] = { 0 };
                run(out,idx,1,I,n,j,degree);
                return out[0];
            }

            template <typename ITERATOR>
            T operator()(const ITERATOR I,
                         const size_t   n,
                         const size_t   j,
                         const size_t   degree,
                         T             &drvs)
            {
                static const size_t idx[2] = { 0,1 };
                T                   out[2] = { 0,0 };
                run(out,idx,2,I,n,j,degree);
                drvs = out[1];
                return out[0];
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(SavGol);
            class  Code;
            Code  *code;
        public:
            XAdd  &xadd;
        };
    }

}

#endif

