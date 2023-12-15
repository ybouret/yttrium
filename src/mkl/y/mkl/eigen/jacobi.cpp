#include "y/mkl/eigen/jacobi.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/utils.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Eigen
        {
#define ROTATE(a,i,j,k,l) Rotate(a,i,j,k,l,s,tau)

            //______________________________________________________________________
            //
            //
            //! Jacobi's Method implementation
            //
            //______________________________________________________________________
            template <typename T>
            class Jacobi<T>::Code : public Object
            {
            public:

                //__________________________________________________________________
                //
                //
                // C++
                //
                //__________________________________________________________________
                inline explicit Code() :
                xadd(),
                data(),
                zero(0),
                one(1),
                half(0.5),
                fifth(0.2),
                big(100.0)
                {}

                inline virtual ~Code() noexcept {}


                //__________________________________________________________________
                //
                //
                // Members
                //
                //__________________________________________________________________
                Antelope::Add<T> xadd;
                Vector<T>        data;
                const T          zero;
                const T          one;
                const T          half;
                const T          fifth;
                const T          big;

                //__________________________________________________________________
                //
                //
                // Methods
                //
                //__________________________________________________________________

                //! specialized fabs
                static inline T FabsOf(const T x) { return Fabs<T>::Of(x); }

                //! specialized sqrt
                static inline T SqrtOf(const T x) { return Sqrt<T>::Of(x); }

                //! check if value is unchanged
                inline bool unchanged(const T &value,
                                      const T &delta) const
                {
                    const T absv = FabsOf(value);
                    const T absa = OutOfReach::Add(absv,delta);
                    const T diff = FabsOf(absa-absv);
                    return diff <= zero;
                }

                //! rotation
                static inline void Rotate(Matrix<T>   &a,
                                          const size_t i,
                                          const size_t j,
                                          const size_t k,
                                          const size_t l,
                                          const T      &s,
                                          const T      &tau) noexcept
                {
                    const T g=a[i][j];
                    const T h=a[k][l];
                    a[i][j]=g-s*(h+g*tau);
                    a[k][l]=h+s*(g-h*tau);
                }

                static inline void Rebuild(Matrix<T> &a) noexcept
                {
                    assert(a.isSquare());
                    const size_t n = a.rows;
                    for(size_t i=1;i<n;++i)
                    {
                        for(size_t j=i+1;j<=n;++j)
                        {
                            a[i][j] = a[j][i];
                        }
                    }
                }


                // algorithm
                inline bool process(Matrix<T>   &a,
                                    Writable<T> &d,
                                    Matrix<T>   &v)
                {
                    assert(a.cols==a.rows);
                    assert(v.cols==a.cols);
                    assert(v.rows==a.rows);
                    assert(d.size()==a.rows);

                    //----------------------------------------------------------
                    // prepare resources
                    //----------------------------------------------------------
                    const size_t n  = a.rows; assert(n>0);
                    const T      n2 = n*n;
                    data.adjust(2*n,zero);
                    LightArray<T> b(&data[1],n);
                    LightArray<T> z(&data[n+1],n);

                    //----------------------------------------------------------
                    //  initialize
                    //----------------------------------------------------------
                    for(size_t i=n;i>0;--i)
                    {
                        for(size_t j=n;j>0;--j) v[i][j] = zero;
                        b[i]    = d[i] = a[i][i];
                        v[i][i] = one;
                        z[i]    = zero;
                    }

                    //----------------------------------------------------------
                    //  Loop and Rebuild(a) in any case
                    //----------------------------------------------------------
                    try
                    {
                        for(size_t i=0;i<128;++i)
                        {
                            //--------------------------------------------------
                            // test convergence
                            //--------------------------------------------------
                            xadd.free();
                            for (size_t ip=1;ip<n;++ip)
                            {
                                for (size_t iq=ip+1;iq<=n;++iq)
                                {
                                    xadd << Fabs<T>::Of(a[ip][iq]);
                                }
                            }
                            const T sm = xadd.sum();
                            if( Fabs<T>::Of(sm) <= zero )
                            {
                                Rebuild(a);
                                return true;
                            }

                            //--------------------------------------------------
                            // sweep
                            //--------------------------------------------------
                            const T tresh = (i<4) ? (fifth*sm)/n2 : zero;
                            for(size_t ip=1;ip<=n;++ip)
                            {
                                for(size_t iq=ip+1;iq<=n;++iq)
                                {
                                    T       t = zero;
                                    const T g = big * Fabs<T>::Of(a[ip][iq]);
                                    if (i > 4
                                        && unchanged(d[ip],g)
                                        && unchanged(d[iq],g) )
                                    {
                                        a[ip][iq]=zero;
                                    }
                                    else
                                    {
                                        if( Fabs<T>::Of(a[ip][iq]) > tresh)
                                        {
                                            const T hh=d[iq]-d[ip];
                                            if (unchanged(hh,g))
                                                t=(a[ip][iq])/hh;
                                            else
                                            {
                                                const T theta=half*hh/(a[ip][iq]);
                                                t=one/(Fabs<T>::Of(theta)+SqrtOf(one+theta*theta));
                                                if (theta<zero) t = -t;
                                            }

                                            const T c   = one/ SqrtOf(one+t*t);
                                            const T s   = t*c;
                                            const T tau = s/(one+c);
                                            const T h   = t*a[ip][iq];

                                            z[ip] -= h;
                                            z[iq] += h;
                                            d[ip] -= h;
                                            d[iq] += h;
                                            a[ip][iq]=zero;

                                            for(size_t j=1;j<ip;j++)    ROTATE(a,j,ip,j,iq);
                                            for(size_t j=ip+1;j<iq;j++) ROTATE(a,ip,j,j,iq);
                                            for(size_t j=iq+1;j<=n;j++) ROTATE(a,ip,j,iq,j);
                                            for(size_t j=1;j<=n;j++)    ROTATE(v,j,ip,j,iq);
                                        }
                                    }
                                }
                            }

                            for(size_t ip=n;ip>0;--ip)
                            {
                                b[ip] += z[ip];
                                d[ip]=b[ip];
                                z[ip]=zero;
                            }
                        }
                    }
                    catch(...)
                    {
                        Rebuild(a);
                        throw;
                    }

                    Rebuild(a);
                    return false;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };


#define real_t float
#include "jacobi.hxx"

#undef  real_t
#define real_t double
#include "jacobi.hxx"

#undef  real_t
#define real_t long double
#include "jacobi.hxx"

#undef  real_t
#define real_t XReal<float>
#include "jacobi.hxx"

#undef  real_t
#define real_t XReal<double>
#include "jacobi.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "jacobi.hxx"

        }
    }
}
