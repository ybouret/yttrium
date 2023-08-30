
#include "y/mkl/lak/lu.hpp"
#include "y/mkl/api.hpp"
#include "y/type/utils.hpp"
#include "y/container/cxx-array.hpp"

namespace Yttrium
{

    namespace MKL
    {

        template <typename T>
        class LU<T> :: Code : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef T                           Type;    //!< hold values
            typedef typename ScalarFor<T>::Type Scalar;  //!< hold absolute values
            typedef Memory::Dyadic              MemMgr;  //!< memory model
            typedef CxxArray<Scalar,MemMgr>     Scalars; //!< requested size
            typedef CxxArray<size_t,MemMgr>     Indices; //!< requested size

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Code(const size_t nmin) :
            s0(0),
            s1(1),
            t1(1),
            dpos(true),
            scal(nmin),
            indx(nmin)
            {
                assert(nmin>0);
                assert(scal.size() == nmin);
                assert(indx.size() == nmin);
            }

            virtual ~Code() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool build(Matrix<T> &a)
            {
                assert(a.isSquare());
                assert(a.rows>0);
                assert(a.rows<=scal.size());
                assert(a.rows<=indx.size());

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                const size_t n = a.size();
                dpos=true;

                //--------------------------------------------------------------
                //
                // check scaling
                //
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i)
                {
                    Scalar                piv = s0;
                    const Readable<Type> &a_i = a[i];
                    for(size_t j=n;j>0;--j)
                    {
                        const Scalar tmp = Fabs<Type>::Of(a_i[j]);
                        if(tmp>piv) piv = tmp;
                    }
                    if(piv<=s0) return false;
                    scal[i] = s1/piv;
                }

                //--------------------------------------------------------------
                //
                // Crout's algorithm
                //
                //--------------------------------------------------------------
                for(size_t j=1;j<=n;++j)
                {
                    for (size_t i=1;i<j;i++)
                    {
                        Type sum=a[i][j];
                        for(size_t k=1;k<i;k++)
                            sum -= a[i][k]*a[k][j];
                        a[i][j]=sum;
                    }

                    Scalar big=s0;
                    size_t imax=j;
                    for (size_t i=j;i<=n;i++)
                    {
                        Type sum=a[i][j];
                        for (size_t k=1;k<j;k++)
                            sum -= a[i][k]*a[k][j];
                        a[i][j]=sum;
                        const Scalar dum = scal[i] * Fabs<Type>::Of(sum);
                        if( dum >= big)
                        {
                            big  = dum;
                            imax = i;
                        }
                    }

                    if (j != imax) {
                        a.swapRows(j,imax);
                        Swap(scal[imax],scal[j]);
                        dpos = !dpos;
                        //scal[imax]=scal[j];
                    }

                    indx[j]=imax;

                    if( Fabs<Type>::Of(a[j][j]) <= s0 ) return false;

                    if (j != n) {
                        const Type factor= t1/a[j][j];
                        for (size_t i=j+1;i<=n;i++)
                            a[i][j] *= factor;
                    }
                }

                return true;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Scalar s0;     //!< Scalar(0)
            const Scalar s1;     //!< Scalar(1)
            const Type   t1;     //!< Type(1)
            bool         dpos;   //!< sign of determinant is positive
            Scalars      scal;   //!< scaling factors
            Indices      indx;   //!< swap indices

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

#define real_t float
#include "lu.hxx"

#undef  real_t
#define real_t double
#include "lu.hxx"

#undef  real_t
#define real_t long double
#include "lu.hxx"

#undef  real_t
#define real_t XReal<float>
#include "lu.hxx"

#undef  real_t
#define real_t XReal<double>
#include "lu.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "lu.hxx"

#undef  real_t
#define real_t Complex<float>
#include "lu.hxx"

#undef  real_t
#define real_t Complex<double>
#include "lu.hxx"

#undef  real_t
#define real_t Complex<long double>
#include "lu.hxx"


    }

}


