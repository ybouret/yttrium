
inline bool isSmall(const T small, const T &big) noexcept
{
    const T sum = OutOfReach::Add(small,big);
    const T dif = sum - big;
    return Fabs<T>::Of(dif) <= zero;
}

static inline T SqrtOf(const T value) { return Sqrt<T>::Of(value); }

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
    static const T half(0.5);
    assert( a.isSquare() );
    assert( a.rows>0     );
    const ptrdiff_t n = a.rows;
    assert( wr.size()   >= a.rows );
    assert( wi.size()   >= a.rows );

    ptrdiff_t nn,m,l,k,j,i,mmin;
    T         z,y,x,w,v,u,t,s,r=0,q=0,p=0,anorm;

    size_t   ir = 1; //! where to put real eigenvalues
    size_t   ic = n; //! where to put cplx eigenvalues
    nr    = 0;
    anorm = 0;
    for (i=1;i<=n;i++)
    {
        for (j=Max<size_t>(i-1,1);j<=n;j++)
            anorm += Fabs<T>::Of(a[i][j]);
    }
    nn=n;
    t=zero;
    while(nn>=1)
    {
        unsigned its=0;
        do
        {
            for (l=nn;l>=2;l--)
            {
                s = Fabs<T>::Of(a[l-1][l-1])+Fabs<T>::Of(a[l][l]);
                if (s <= zero)
                    s=anorm;
                if( isSmall(Fabs<T>::Of(a[l][l-1]),s))
                    break;
            }
            x=a[nn][nn];
            if (l == nn)
            {
                wr[ir]=x+t;
                wi[ir]=zero;
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
                    p=half*(y-x);
                    q=p*p+w;
                    const T absq = Fabs<T>::Of(q);
                    z=Sqrt<T>::Of(absq);
                    x += t;
                    if (q >= zero)
                    {
                        z = p + Sgn(z,p);
                        wr[ir+1]=wr[ir]=x+z;
                        if( Fabs<T>::Of(z)>zero )
                            wr[ir]=x-w/z;
                        wi[ir+1]=wi[ir]=zero;
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
                        static const T _3over4(0.75);
                        static const T _minus7over16(-0.4375);

                        t += x;
                        for (i=1;i<=nn;i++)
                            a[i][i] -= x;
                        s=Fabs<T>::Of(a[nn][nn-1])+Fabs<T>::Of(a[nn-1][nn-2]);
                        y = x = _3over4*s;
                        w     = _minus7over16*s*s;
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
                        s=Antelope::Sum3<T>::OfAbs(p,q,r);
                        p /= s;
                        q /= s;
                        r /= s;
                        if (m == l)
                        {
                            break;
                        }
                        u=Fabs<T>::Of(a[m][m-1])*(Fabs<T>::Of(q)+Fabs<T>::Of(r));
                        v=Fabs<T>::Of(p)*(Antelope::Sum3<T>::OfAbs(a[m-1][m-1],z,a[m+1][m+1]));
                        if(isSmall(u,v))
                            break;
                    }
                    for(i=m+2;i<=nn;i++)
                    {
                        a[i][i-2]=zero;
                        if (i != (m+2))
                            a[i][i-3]=zero;
                    }
                    for(k=m;k<=nn-1;++k)
                    {
                        if (k != m)
                        {
                            p=a[k][k-1];
                            q=a[k+1][k-1];
                            r=zero;
                            if (k != (nn-1)) r=a[k+2][k-1];
                            if ( (x=Antelope::Sum3<T>::OfAbs(p,q,r))>zero )
                            {
                                p /= x;
                                q /= x;
                                r /= x;
                            }
                        }
                        if( Fabs<T>::Of(s=Sgn(Hypotenuse(p,q,r),p)) > zero )
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

    // sort real values
    if(nr>0)
    {
        LightArray<T> W(&wr[1],nr);
        HeapSort::Call(W, Comparison::Increasing<T> );
    }

    return true;
}
