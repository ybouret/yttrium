
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

                static const unsigned MAX_ITS =100; //!< maximum number of cycles
                static const unsigned SCALING = 10; //!< scaling every cycle
                //! find the eigen values
                /**
                 \param a  a real matrix reduced to its Hessenberg form: destructed !
                 \param wr an array that will be filled with the real parts
                 \param wi an array that will be filled with the imagnary parts
                 \param nr the number or real eigenvalues
                 wi[1..nr]=0 and wr[1..nr] are sorted by increasing order.
                 */
                inline bool QR( Matrix<T> &a, Writable<T> &wr, Writable<T> &wi, size_t &nr)
                {
                    assert( a.is_square );
                    assert( a.rows>0    );
                    const ptrdiff_t n = a.rows;
                    assert( wr.size()   >= a.rows );
                    assert( wi.size()   >= a.rows );
                    //assert( flag.size() >= n );

                    ptrdiff_t nn,m,l,k,j,i,mmin;
                    T         z,y,x,w,v,u,t,s,r=0,q=0,p=0,anorm;

                    size_t   ir = 1; //! where to put real eigenvalues
                    size_t   ic = n; //! where to put cplx eigenvalues
                    nr    = 0;
                    anorm = 0;
                    for (i=1;i<=n;i++)
                        for (j=Max<size_t>(i-1,1);j<=n;j++)
                            anorm += fabs_of(a[i][j]);
                    nn=n;
                    t=0;
                    while(nn>=1)
                    {
                        unsigned its=0;
                        do
                        {
                            for (l=nn;l>=2;l--)
                            {
                                s=fabs_of(a[l-1][l-1])+fabs_of(a[l][l]);
                                if (s <= 0)
                                    s=anorm;
                                if ((T)(fabs_of(a[l][l-1]) + s) == s)
                                    break;
                            }
                            x=a[nn][nn];
                            if (l == nn)
                            {
                                wr[ir]=x+t;
                                wi[ir]=0;
                                //std::cerr << "#EIG: real single: " << wr[ir] << std::endl;
                                ++ir;
                                ++nr;
                                --nn;
                            }
                            else
                            {
                                y=a[nn-1][nn-1];
                                w=a[nn][nn-1]*a[nn-1][nn];
                                if(l == (nn-1))
                                {
                                    p=T(0.5)*(y-x);
                                    q=p*p+w;
                                    z=sqrt_of(fabs_of(q));
                                    x += t;
                                    if (q >= 0)
                                    {
                                        z=p+__sgn(z,p);
                                        wr[ir+1]=wr[ir]=x+z;
                                        if( fabs_of(z)>0 )
                                            wr[ir]=x-w/z;
                                        //std::cerr << "#EIG: real pair: " << wr[ir] << ", " << wr[ir+1] << ", x=" << x << ", w=" << w << ", z=" << z << ", p=" << p << ", sq=" << Sqrt(Fabs(q)) << std::endl;
                                        wi[ir+1]=wi[ir]=0;
                                        ir += 2;
                                        nr += 2;
                                    }
                                    else
                                    {
                                        wr[ic-1]=wr[ic]=x+p;
                                        wi[ic-1]= -(wi[ic]=z);
                                        ic -= 2;
                                    }
                                    nn -= 2;
                                }
                                else
                                {
                                    if (its >= MAX_ITS)
                                    {
                                        return false;
                                    }
                                    if (0 == (its%SCALING) )
                                    {
                                        t += x;
                                        for (i=1;i<=nn;i++)
                                            a[i][i] -= x;
                                        s=fabs_of(a[nn][nn-1])+fabs_of(a[nn-1][nn-2]);
                                        y=x= T(0.75)*s;
                                        w = -T(0.4375)*s*s;
                                    }
                                    ++its;
                                    for(m=(nn-2);m>=l;m--)
                                    {
                                        z=a[m][m];
                                        r=x-z;
                                        s=y-z;
                                        p=(r*s-w)/a[m+1][m]+a[m][m+1];
                                        q=a[m+1][m+1]-z-r-s;
                                        r=a[m+2][m+1];
                                        s=fabs_of(p)+fabs_of(q)+fabs_of(r);
                                        p /= s;
                                        q /= s;
                                        r /= s;
                                        if (m == l)
                                        {
                                            break;
                                        }
                                        u=fabs_of(a[m][m-1])*(fabs_of(q)+fabs_of(r));
                                        v=fabs_of(p)*(fabs_of(a[m-1][m-1])+fabs_of(z)+fabs_of(a[m+1][m+1]));
                                        if ((T)(u+v) == v)
                                        {
                                            break;
                                        }
                                    }
                                    for (i=m+2;i<=nn;i++)
                                    {
                                        a[i][i-2]=0;
                                        if (i != (m+2))
                                            a[i][i-3]=0;
                                    }
                                    for (k=m;k<=nn-1;k++)
                                    {
                                        if (k != m)
                                        {
                                            p=a[k][k-1];
                                            q=a[k+1][k-1];
                                            r=0;
                                            if (k != (nn-1)) r=a[k+2][k-1];
                                            if ( (x=fabs_of(p)+fabs_of(q)+fabs_of(r))>0 )
                                            {
                                                p /= x;
                                                q /= x;
                                                r /= x;
                                            }
                                        }
                                        if( fabs_of(s=__sgn(sqrt_of(p*p+q*q+r*r),p)) > 0 )
                                        {
                                            if (k == m)
                                            {
                                                if (l != m)
                                                    a[k][k-1] = -a[k][k-1];
                                            }
                                            else
                                            {
                                                a[k][k-1] = -s*x;
                                            }
                                            p += s;
                                            x=p/s;
                                            y=q/s;
                                            z=r/s;
                                            q /= p;
                                            r /= p;
                                            for(j=k;j<=nn;j++)
                                            {
                                                p=a[k][j]+q*a[k+1][j];
                                                if (k != (nn-1))
                                                {
                                                    p += r*a[k+2][j];
                                                    a[k+2][j] -= p*z;
                                                }
                                                a[k+1][j] -= p*y;
                                                a[k][j]   -= p*x;
                                            }
                                            mmin = nn<k+3 ? nn : k+3;
                                            for (i=l;i<=mmin;i++)
                                            {
                                                p=x*a[i][k]+y*a[i][k+1];
                                                if (k != (nn-1)) {
                                                    p += z*a[i][k+2];
                                                    a[i][k+2] -= p*r;
                                                }
                                                a[i][k+1] -= p*q;
                                                a[i][k] -= p;
                                            }
                                        }
                                    }
                                }
                            }
                        } while (l < nn-1);
                    }
                    LightArray<T> W(*wr,nr);
                    //hsort(W,comparison::increasing<T>);
                    return true;
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

