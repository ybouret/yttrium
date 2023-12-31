
#include "y/mkl/algebra/lu.hpp"
#include "y/type/utils.hpp"
#include "y/container/cxx/array.hpp"

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
            typedef CxxArray<Type,MemMgr>       Types;   //!< request size, helper

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Code(const size_t nmin) :
            s0(0),
            s1(1),
            t0(0),
            t1(1),
            dpos(true),
            scal(nmin),
            indx(nmin),
            aux(nmin),
            xadd(nmin),
            xmul(nmin)
            {
                assert(nmin>0);
                assert(scal.size() == nmin);
                assert(indx.size() == nmin);
                assert(aux.size()  == nmin);
            }

            virtual ~Code() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //
            //! Decomposition
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
#if 0
                        Type sum=a[i][j];
                        for(size_t k=1;k<i;k++)
                            sum -= a[i][k]*a[k][j];
                        a[i][j]=sum;
#endif
                        MatrixRow<T> &a_i  = a[i];
                        T            &a_ij = a_i[j];
                        xadd.free();
                        xadd << a_ij;
                        for(size_t k=1;k<i;k++)
                            xadd << -a_i[k] * a[k][j];
                        a_ij = xadd.sum();
                    }

                    Scalar big=s0;
                    size_t imax=j;
                    for (size_t i=j;i<=n;i++)
                    {
#if 0
                        Type sum=a[i][j];
                        for (size_t k=1;k<j;k++)
                            sum -= a[i][k]*a[k][j];
                        a[i][j]=sum;
#endif

                        MatrixRow<T> &a_i  = a[i];
                        T            &a_ij = a_i[j];
                        xadd.free();
                        xadd << a_ij;
                        for (size_t k=1;k<j;k++)
                            xadd << -a[i][k]*a[k][j];
                        const Scalar dum = scal[i] * Fabs<Type>::Of(a_ij=xadd.sum());
                        if( dum >= big)
                        {
                            big  = dum;
                            imax = i;
                        }
                    }

                    if (j != imax) {
                        a.swapRows(j,imax);
                        dpos = !dpos;
                        scal[imax]=scal[j];
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
            // In place solve
            //
            //__________________________________________________________________
            inline void solve(const Matrix<T> &a, Writable<T> &b)
            {
                assert(a.isSquare());
                assert(a.rows>0);
                assert(a.rows<=scal.size());
                assert(a.rows<=indx.size());
                assert(b.size()>=a.rows);


                const size_t n  = a.rows;
                size_t       ii = 0;
                for(size_t i=1;i<=n;i++)
                {
                    const size_t ip  = indx[i];
                    Type         sum = b[ip];
                    b[ip]=b[i];
                    if(ii>0)
                    {
#if 0
                        for(size_t j=ii;j<i;j++) sum -= a[i][j]*b[j];
#endif
                        xadd.free();
                        xadd << sum;
                        {
                            const Readable<T> &a_i = a[i];
                            for(size_t j=ii;j<i;j++) xadd << -a_i[j]*b[j];
                        }
                        sum = xadd.sum();
                    }
                    else
                    {
                        if ( Fabs<Type>::Of(sum)>s0)
                            ii=i;
                    }
                    b[i]=sum;
                }

                for(size_t i=n;i>=1;i--)
                {
#if 0
                    Type sum=b[i];
                    for(size_t j=i+1;j<=n;j++) sum -= a[i][j]*b[j];
                    b[i]=sum/a[i][i];
#endif

                    const Readable<T> &a_i = a[i];
                    xadd.free();
                    xadd << b[i];
                    for(size_t j=n;j>i;--j)
                        xadd << - a_i[j] * b[j];
                    b[i] = xadd.sum()/a_i[i];

                }
            }

            //__________________________________________________________________
            //
            //
            //! solve all columns of b
            //
            //__________________________________________________________________
            inline void solve(const Matrix<T> &a, Matrix<T> &b)
            {
                assert(a.isSquare());
                assert(a.rows>0);
                assert(a.rows<=scal.size());
                assert(b.rows == a.rows);
                const size_t n = a.rows;
                for(size_t j=b.cols;j>0;--j)
                {
                    for(size_t k=n;k>0;--k)
                        aux[k] = b[k][j];
                    solve(a,aux);
                    for(size_t k=n;k>0;--k)
                        b[k][j] = aux[k];
                }
            }

            //__________________________________________________________________
            //
            //
            //! invert of a
            //
            //__________________________________________________________________
            inline void invert(const Matrix<T> &a,
                               Matrix<T>       &b)
            {
                assert(a.isValid());
                assert(a.isSquare());
                assert(a.rows>0);
                assert(a.rows<=scal.size());
                assert(a.hasSameMetricsThan(b));
                const size_t n=a.rows;
                for(size_t j=n;j>0;--j)
                {
                    aux.ld(t0);
                    aux[j]=t1;
                    solve(a,aux);
                    for(size_t i=n;i>0;--i) b[i][j] = aux[i];
                }
            }

            //__________________________________________________________________
            //
            //
            // Determinant of a
            //
            //__________________________________________________________________
            inline T det(const Matrix<T> &a)
            {
                assert(a.isValid());
                assert(a.isSquare());
                assert(a.rows>0);
                assert(a.rows<=scal.size());
                xmul.free();
                for(size_t i=a.rows;i>0;--i) xmul << a[i][i];
                return dpos ? xmul.product() : -xmul.product();
            }

            //__________________________________________________________________
            //
            //
            // tranpose(CoMatrix)
            //
            //__________________________________________________________________
            inline void ajdoint(Matrix<T>       &adj,
                                const Matrix<T> &a)
            {
                assert(a.isValid());
                assert(a.isSquare());
                assert(a.rows>=2);
                assert(scal.size()>=a.rows-1);
                assert(adj.hasSameMetricsThan(a));

                const size_t n  = a.rows;
                const size_t nm = n-1;
                Matrix<T>    sub(nm,nm);
                for(size_t i=n;i>0;--i)
                {
                    for(size_t j=n;j>0;--j)
                    {
                        a.buildMinor(sub,i,j);
                        if(build(sub))
                        {
                            const bool positive = 0 == (0x01 & (i+j));
                            adj[j][i] = positive ? det(sub) : -det(sub);
                        }
                        else
                        {
                            adj[j][i] = 0;
                        }
                    }
                }
                
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Scalar     s0;     //!< Scalar(0)
            const Scalar     s1;     //!< Scalar(1)
            const Type       t0;     //!< Type(0);
            const Type       t1;     //!< Type(1)
            bool             dpos;   //!< sign of determinant is positive
            Scalars          scal;   //!< scaling factors
            Indices          indx;   //!< swap indices
            Types            aux;    //!< auxiliary vector
            Antelope::Add<T> xadd;   //!< add
            Antelope::Mul<T> xmul;   //!< mul

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
#define real_t apq
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

#undef  real_t
#define real_t Complex< XReal<float> >
#include "lu.hxx"

#undef  real_t
#define real_t Complex< XReal<double> >
#include "lu.hxx"


#undef  real_t
#define real_t Complex< XReal<long double> >
#include "lu.hxx"

    }

}


