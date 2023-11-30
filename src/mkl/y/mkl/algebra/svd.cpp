#include "y/mkl/algebra/svd.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/utils.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{
    namespace MKL
    {
        template <typename T>
        class SVD<T>:: Code : public Object
        {
        public:
            inline explicit Code() :
            Object(),
            zero(0),
            one(1),
            rv1()
            {
            }

            inline virtual ~Code() noexcept {}

            const T   zero;
            const T   one;
            Vector<T> rv1;

            static const size_t MAX_ITS = 1024; //!< maximum iterations

            static inline T FabsOf(const T x) { return Fabs<T>::Of(x); }
            static inline T SqrtOf(const T x) { return Sqrt<T>::Of(x); }

            //! singular value decomposition
            /**
             -Given a matrix a[1..m][1..n], this routine computes its singular value
             decomposition, a = u.w.v'.
             - The matrix u replaces a on output.  The diagonal
             matrix of singular values w is output as a vector w[1..n].
             - The matrix v (not the transpose v') is output as v[1..n][1..n].
             */
            inline bool build(Matrix<T> &a, Writable<T> &w, Matrix<T> &v)
            {
                const size_t m = a.rows;
                const size_t n = a.cols;
                assert(v.cols==a.cols);
                assert(v.rows==a.cols);
                assert(w.size()==a.cols);
                rv1.adjust(n,zero);

                size_t k;
                T      c,f,h,s,x,y,z;
                size_t l = 0;
                T      g=zero,scale=zero,anorm=zero; /* Householder reduction to bidiagonal form */
               
                for(size_t i=1;i<=n;i++)
                {
                    l=i+1;
                    rv1[i]=scale*g;
                    g=s=scale=zero;
                    if (i <= m)
                    {
                        for(k=i;k<=m;++k)
                        {
                            scale += FabsOf(a[k][i]);
                        }
                        if(scale>zero)
                        {
                            for(k=i;k<=m;k++)
                            {
                                a[k][i] /= scale;
                                s += a[k][i]*a[k][i];
                            }
                            f = a[i][i];
                            g = - Sgn(SqrtOf(s),f);
                            h = f*g-s;
                            a[i][i]=f-g;
                            for(size_t j=l;j<=n;++j)
                            {
                                for(s=0.0,k=i;k<=m;++k)
                                    s += a[k][i]*a[k][j];
                                f=s/h;
                                for(k=i;k<=m;++k)
                                    a[k][j] += f*a[k][i];
                            }
                            for (k=i;k<=m;++k) a[k][i] *= scale;
                        }
                    }
                    w[i]=scale *g;
                    g=s=scale=zero;
                    if (i <= m && i != n)
                    {
                        for(k=l;k<=n;++k)
                        {
                            scale += FabsOf(a[i][k]);
                        }
                        if(scale>zero)
                        {
                            for(k=l;k<=n;++k)
                            {
                                a[i][k] /= scale;
                                s += a[i][k]*a[i][k];
                            }
                            f = a[i][l];
                            g = -Sgn(SqrtOf(s),f);
                            h=f*g-s;
                            a[i][l]=f-g;
                            for(k=l;k<=n;k++)
                                rv1[k]=a[i][k]/h;
                            for(size_t j=l;j<=m;++j) {
                                for (s=zero,k=l;k<=n;k++)
                                    s += a[j][k]*a[i][k];
                                for(k=l;k<=n;k++)
                                    a[j][k] += s*rv1[k];
                            }
                            for (k=l;k<=n;k++)
                                a[i][k] *= scale;
                        }
                    }
                    anorm = Max<T>(anorm,(FabsOf(w[i])+FabsOf(rv1[i])));
                }
                for(size_t i=n;i>=1;--i)
                {
                    if (i<n)
                    {
                        if(FabsOf(g)>zero)
                        {
                            for(size_t j=l;j<=n;++j)
                                v[j][i]=(a[i][j]/a[i][l])/g;
                            for(size_t j=l;j<=n;j++)
                            {
                                for (s=0.0,k=l;k<=n;++k)
                                    s += a[i][k]*v[k][j];
                                for (k=l;k<=n;++k)
                                    v[k][j] += s*v[k][i];
                            }
                        }
                        for(size_t j=l;j<=n;j++)
                            v[i][j]=v[j][i]=zero;
                    }
                    v[i][i]=one;
                    g=rv1[i];
                    l=i;
                }
                for(size_t i=Min(m,n);i>=1;i--)
                {
                    /* Accumulation of left-hand transformations. */
                    l=i+1;
                    g=w[i];
                    for(size_t j=l;j<=n;j++)
                        a[i][j]=zero;
                    if(FabsOf(g)>zero)
                    {
                        g=T(1)/g;
                        for(size_t j=l;j<=n;j++)
                        {
                            for (s=0,k=l;k<=m;k++) s += a[k][i]*a[k][j];
                            f=(s/a[i][i])*g;
                            for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
                        }
                        for(size_t j=i;j<=m;j++) a[j][i] *= g;
                    }
                    else
                        for(size_t j=i;j<=m;j++) a[j][i]=zero;
                    //++a[i][i];
                    a[i][i] += one;
                }
                for (k=n;k>=1;k--)
                {
                    /* Diagonalization of the bidiagonal form. */
                    unsigned its=0;
                    for(its=1;its<=MAX_ITS;its++)
                    {
                        int   flag = 1;
                        size_t nm  = 0;
                        for (l=k;l>=1;l--)
                        {
                            /* Test for splitting. */
                            nm=l-1;

                            /* Note that rv1[1] is always zero. */
                            if ((T)(FabsOf(rv1[l])+anorm) == anorm)
                            {
                                flag=0;
                                break;
                            }
                            if ((T)(FabsOf(w[nm])+anorm) == anorm) break;
                        }
                        if (flag)
                        {
                            c=zero; /* Cancellation of rv1[l], if l > 1. */
                            s=one;
                            for(size_t i=l;i<=k;i++) {
                                f=s*rv1[i];
                                rv1[i]=c*rv1[i];
                                if ((T)(FabsOf(f)+anorm) == anorm) break;
                                g=w[i];
                                h=Hypotenuse(f,g);
                                w[i]=h;
                                h=one/h;
                                c=g*h;
                                s = -f*h;
                                for(size_t j=1;j<=m;j++) {
                                    y=a[j][nm];
                                    z=a[j][i];
                                    a[j][nm]=y*c+z*s;
                                    a[j][i]=z*c-y*s;
                                }
                            }
                        }
                        z=w[k];
                        if (l == k) { /* Convergence. */
                            if(z<zero) { /* Singular value is made nonnegative. */
                                w[k] = -z;
                                for(size_t j=1;j<=n;j++) v[j][k] = -v[j][k];
                            }
                            break;
                        }
                        if (its >= MAX_ITS)
                        {
                            return false; // no convergence
                        }
                        x=w[l]; /* Shift from bottom 2-by-2 minor. */
                        nm=k-1;
                        y=w[nm];
                        g=rv1[nm];
                        h=rv1[k];
                        f=((y-z)*(y+z)+(g-h)*(g+h))/(T(2)*h*y);
                        g=Hypotenuse(f,T(1));
                        f=((x-z)*(x+z)+h*((y/(f+Sgn(g,f)))-h))/x;
                        c=s=1; /* Next QR transformation: */
                        for(size_t j=l;j<=nm;j++)
                        {
                            const size_t i=j+1;
                            g=rv1[i];
                            y=w[i];
                            h=s*g;
                            g=c*g;
                            z=Hypotenuse(f,h);
                            rv1[j]=z;
                            c=f/z;
                            s=h/z;
                            f=x*c+g*s;
                            g=g*c-x*s;
                            h=y*s;
                            y *= c;
                            for(size_t jj=1;jj<=n;jj++)
                            {
                                x=v[jj][j];
                                z=v[jj][i];
                                v[jj][j]=x*c+z*s;
                                v[jj][i]=z*c-x*s;
                            }
                            z=Hypotenuse(f,h);
                            w[j]=z; /* Rotation can be arbitrary if z = 0. */
                            if(z>zero) {
                                z=T(1)/z;
                                c=f*z;
                                s=h*z;
                            }
                            f=c*g+s*y;
                            x=c*y-s*g;
                            for(size_t jj=1;jj<=m;++jj)
                            {
                                y=a[jj][j];
                                z=a[jj][i];
                                a[jj][j]=y*c+z*s;
                                a[jj][i]=z*c-y*s;
                            }
                        }
                        rv1[l]=zero;
                        rv1[k]=f;
                        w[k]=x;
                    }
                }
                return true;
            }

#if 0
            //! solve a system after a singular value decomposition
            /**
             Solves A·X = B for a vector X, where A is specified
             by the arrays u[1..m][1..n], w[1..n], v[1..n][1..n] as returned by svdcmp.
             m and n are the dimensions of a,
             and will be equal for square matrices.
             b[1..m] is the input right-hand side.
             x[1..n] is the output solution vector.
             No input quantities are destroyed, so the routine may be called sequentially with different b’s.
             */
            template <typename T> static inline
            void solve(const matrix<T>      &u,
                       const accessible<T>  &w,
                       const matrix<T>      &v,
                       const accessible<T>  &b,
                       addressable<T>       &x)
            {
                const size_t m = u.rows;
                const size_t n = u.cols;
                assert( w.size() == n );
                assert( v.rows == n);
                assert( v.cols == n );

                array<T> &tmp = u.c_aux1;
                for(size_t j=n;j>0;--j) {
                    T s=0;
                    if( fabs_of(w[j])>0 )
                    {
                        for(size_t i=m;i>0;--i) s += u[i][j]*b[i];
                        s /= w[j];
                    }
                    tmp[j]=s;
                }
                for(size_t j=n;j>0;--j)
                {
                    T s=0;
                    for(size_t jj=n;jj>0;--jj) s += v[j][jj]*tmp[jj];
                    x[j]=s;
                }
            }

            //! build a supplementary orthonormal basis
            /**
             The rows/or columns of P needs to form a free family
             P.cols>0, P.rows>0 and P.cols != P.rows
             */
            template <typename T> static inline
            bool orthonormal( matrix<T> &Q, const matrix<T> &P)
            {
                //--------------------------------------------------------------
                // sanity check
                //--------------------------------------------------------------
                assert(P.cols>0);
                assert(P.rows>0);
                assert(P.cols!=P.rows);

                //--------------------------------------------------------------
                // check dims
                //--------------------------------------------------------------
                const size_t nr  = P.rows;
                const size_t nc  = P.cols;
                size_t       dim = 0;
                size_t       sub = 0;
                const bool   cols_are_vector = nr>nc;
                if(cols_are_vector)
                {
                    dim = nr;
                    sub = nc;
                }
                else
                {
                    dim = nc;
                    sub = nr;
                }

                //--------------------------------------------------------------
                // Q space
                //--------------------------------------------------------------
                const size_t dof = dim-sub;
                if(cols_are_vector)
                {
                    Q.make(dim,dof);
                }
                else
                {
                    Q.make(dof,dim);
                }

                //--------------------------------------------------------------
                // total space
                //--------------------------------------------------------------
                matrix<T> F(dim,dim);
                if(cols_are_vector)
                {
                    for(size_t v=sub;v>0;--v)
                    {
                        for(size_t i=dim;i>0;--i)
                        {
                            F[i][v] = P[i][v];
                        }
                    }
                }
                else
                {
                    for(size_t v=sub;v>0;--v)
                    {
                        for(size_t i=dim;i>0;--i)
                        {
                            F[i][v] = P[v][i];
                        }
                    }
                }

                //--------------------------------------------------------------
                // use SVD
                //--------------------------------------------------------------
                {
                    vector<T> W(dim);
                    matrix<T> V(dim,dim);
                    if(!build(F,W,V))
                        return false;
                }

                //--------------------------------------------------------------
                // save
                //--------------------------------------------------------------
                if(cols_are_vector)
                {
                    for(size_t v=dof;v>0;--v)
                    {
                        for(size_t i=dim;i>0;--i)
                        {
                            Q[i][v] = F[i][v+sub];
                        }
                    }
                }
                else
                {
                    for(size_t v=dof;v>0;--v)
                    {
                        for(size_t i=dim;i>0;--i)
                        {
                            Q[v][i] = F[i][v+sub];
                        }
                    }

                }

                //--------------------------------------------------------------
                // All done !
                //--------------------------------------------------------------
                return true;
            }
#endif

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };



#define real_t float
#include "svd.hxx"

#undef  real_t
#define real_t double
#include "svd.hxx"

#undef  real_t
#define real_t long double
#include "svd.hxx"

#undef  real_t
#define real_t XReal<float>
#include "svd.hxx"

#undef  real_t
#define real_t XReal<double>
#include "svd.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "svd.hxx"
        
    }

}
