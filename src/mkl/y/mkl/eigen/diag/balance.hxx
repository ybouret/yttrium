
//! balance a real matrix
/**
 Given a matrix a[1..n][1..n], this routine replaces it by a balanced matrix with identical eigenvalues.
 A symmetric matrix is already balanced and is unaffected by this procedure.
 */
inline void balance( Matrix<T> &a )
{
    static const T RADIX  = FLT_RADIX;
    static const T SQRDX  = FLT_RADIX * FLT_RADIX;
    static const T factor(0.95);

    assert( a.isSquare() );
    assert( a.rows>0     );
    const size_t n = a.rows;

    size_t last=0;
    cadd.make(n); assert(cadd.isEmpty());
    radd.make(n); assert(radd.isEmpty());
    while(0==last)
    {
        last=1;
        for(size_t i=1;i<=n;i++)
        {
            assert(cadd.isEmpty());
            assert(radd.isEmpty());
            for(size_t j=n;j>0;--j)
            {
                if (j != i)
                {
                    cadd << Fabs<T>::Of(a[j][i]);
                    radd << Fabs<T>::Of(a[i][j]);
                }
            }
            T r = radd.sum();
            T c = cadd.sum();
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
                if( (c+r)/f < factor*s)
                {
                    last=0;
                    g=one/f;
                    for(size_t j=n;j>0;--j)
                    {
                        a[i][j] *= g;
                        a[j][i] *= f;
                    }
                }
            }
        }
    }
}
