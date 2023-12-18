

template <> Diagonalization<real_t> :: Diagonalization() :
code( new Code() )
{

}

template <> Diagonalization<real_t> :: ~Diagonalization() noexcept
{
    assert(0!=code);
    Nullify(code);
}

template <> void Diagonalization<real_t> :: balance(Matrix<real_t> &a)
{
    assert(code);
    code->balance(a);
}

template <> void Diagonalization<real_t> :: reduce(Matrix<real_t> &a)
{
    assert(code);
    code->reduce(a);
}

#if 0
template <>
void Balance<real_t>(Matrix<real_t> &a)
{
    assert( a.cols == a.rows );

    static const real_t RADIX  = Numeric<real_t>::RADIX;
    static const real_t SQRDX  = Numeric<real_t>::RADIX*Numeric<real_t>::RADIX;
    static const real_t zero   = 0;
    static const real_t one    = 1;
    static const real_t factor = 0.95;
    const        size_t n      = a.cols;

    real_t last=0;
    while(last == zero)
    {
        last=1;
        for(size_t i=1;i<=n;i++)
        {
            real_t r=zero,c=zero;
            for (size_t j=1;j<=n;j++)
            {
                if (j != i)
                {
                    c += fabs(a[j][i]);
                    r += fabs(a[i][j]);
                }
            }
            if (c && r)
            {
                real_t g=r/RADIX;
                real_t f=1.0;
                real_t s=c+r;
                while (c<g) {
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
                    last=zero;
                    g=one/f;
                    for (size_t j=1;j<=n;j++) a[i][j] *= g;
                    for (size_t j=1;j<=n;j++) a[j][i] *= f;
                }
            }
        }
    }
}

#endif

