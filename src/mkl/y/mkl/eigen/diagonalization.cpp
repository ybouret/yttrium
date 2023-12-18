
#include "y/mkl/eigen/diagonalization.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/utils.hpp"
//#include "y/mkl/numeric.hpp"
//#include "y/mkl/antelope/add.hpp"
#include <cfloat>

namespace Yttrium
{
    namespace MKL
    {

        namespace Eigen
        {

            template <typename T>
            class Diagonalization<T> :: Code : public Object
            {
            public:

                inline explicit Code() : Object(),
                zero(0), one(1), balanceFactor(0.95)
                {
                }


                //! balance a real matrix
                /**
                 Given a matrix a[1..n][1..n], this routine replaces it by a balanced matrix with identical eigenvalues.
                 A symmetric matrix is already balanced and is unaffected by this procedure.
                 */
                inline void balance( Matrix<T> &a )
                {
                    static const T RADIX  = FLT_RADIX;
                    static const T SQRDX  = FLT_RADIX * FLT_RADIX;

                    assert( a.isSquare() );
                    assert( a.rows>0     );
                    const size_t n = a.rows;

                    size_t last=0;
                    while(0==last)
                    {
                        last=1;
                        for(size_t i=1;i<=n;i++)
                        {
                            T r=zero,c=zero;
                            for (size_t j=1;j<=n;j++)
                            {
                                if (j != i)
                                {
                                    c += Fabs<T>::Of(a[j][i]);
                                    r += Fabs<T>::Of(a[i][j]);
                                }
                            }
                            if( (c>zero) && (r>zero) )
                            {
                                T g=r/RADIX;
                                T f=one;
                                T s=c+r;
                                while (c<g)
                                {
                                    f *= RADIX;
                                    c *= SQRDX;
                                }
                                g=r*RADIX;
                                while (c>g)
                                {
                                    f /= RADIX;
                                    c /= SQRDX;
                                }
                                if( (c+r)/f < balanceFactor*s)
                                {
                                    last=0;
                                    g=one/f;
                                    for(size_t j=1;j<=n;j++) a[i][j] *= g;
                                    for(size_t j=1;j<=n;j++) a[j][i] *= f;
                                }
                            }
                        }
                    }
                }

                //! reduction to a real Hessenberg form
                /**
                 matrix should be balanced.
                 */
                inline void reduce( Matrix<T> &a ) throw()
                {
                    assert(a.isSquare());
                    const size_t n = a.rows;

                    //----------------------------------------------------------
                    // m = r+1
                    //----------------------------------------------------------
                    for(size_t m=2; m<n; ++m )
                    {
                        const size_t r   = m-1;
                        T            piv = zero;
                        size_t       s   = m;
                        for( size_t j=m+1;j<=n;++j)
                        {
                            //--------------------------------------------------
                            // find the pivot
                            //--------------------------------------------------
                            const T tmp = a[j][r];
                            if(Fabs<T>::Of(tmp)>Fabs<T>::Of(piv))
                            {
                                piv = tmp;
                                s   = j;
                            }
                        }
                        if( s != m )
                        {
                            assert(Fabs<T>::Of(piv)>zero);
                            //--------------------------------------------------
                            // First similarity transform: exchange colums/rows
                            //--------------------------------------------------
                            a.swapBoth(s,m);

                            //--------------------------------------------------
                            // Second similarity transform
                            //--------------------------------------------------
                            assert( Fabs<T>::Of(piv-a[m][m-1]) <= zero );
                            for(size_t i=m+1;i<=n;++i)
                            {
                                const T factor = a[i][r] / piv;

                                //----------------------------------------------
                                // subtract factor times row r + 1 from row i
                                //----------------------------------------------
                                for(size_t j=1;j<=n;++j) a[i][j] -= factor * a[m][j];

                                //----------------------------------------------
                                // add factor times column i to column r + 1
                                //----------------------------------------------
                                for(size_t j=1;j<=n;j++) a[j][m] += factor * a[j][i];
                            }
                        }
                    }

                    //==================================================================
                    // clean up to the exact Hessenberg form
                    //==================================================================
                    for(size_t j=n;j>0;--j)
                    {
                        for(size_t i=j+2;i<=n;++i)
                            a[i][j] = zero;
                    }
                }


                const T zero;
                const T one;
                const T balanceFactor;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

#define real_t float
#include "diagonalization.hxx"

#undef  real_t
#define real_t double
#include "diagonalization.hxx"

#undef  real_t
#define real_t long double
#include "diagonalization.hxx"

#undef  real_t
#define real_t XReal<float>
#include "diagonalization.hxx"

#undef  real_t
#define real_t XReal<double>
#include "diagonalization.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "diagonalization.hxx"
        }

    }

}

