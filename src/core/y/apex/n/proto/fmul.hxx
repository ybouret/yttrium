
#if 0
//! fill real parts of fft
static inline
void FillRe(cplx           * fft,
            const WordType * w,
            const size_t     n) noexcept
{
    size_t j = 0;
    for(size_t i=0;i<n;++i)
    {
        WordType       W = w[i];
        const uint8_t *B = MakeBytes::From(W);
        for(size_t k=0;k<WordSize;++k)
        {
            std::cerr << ' ' << Hexadecimal(B[k]);
            fft[j++].re = B[k];
        }
    }
}

//! fill imaginary parts of fft
static inline
void FillIm(cplx           * fft,
            const WordType * w,
            const size_t     n) noexcept
{
    size_t j = 0;
    for(size_t i=0;i<n;++i)
    {
        WordType       W = w[i];
        const uint8_t *B = MakeBytes::From(W);
        for(size_t k=0;k<WordSize;++k)
        {
            std::cerr << ' ' << Hexadecimal(B[k]);
            fft[j++].im = B[k];
        }
    }
}
#endif

static inline
void FillBatch(Batch<double>  &batch,
               const WordType *w,
               const size_t    nWords,
               const size_t    nBytes)
{
    assert(nWords*WordSize==nBytes);
    size_t j = 0;
    for(size_t i=0;i<nWords;++i)
    {
        WordType              W = w[i];
        const uint8_t * const B = MakeBytes::From(W);
        for(size_t k=0;k<WordSize;++k)
        {
            batch[nBytes-j++] = B[k];
        }
    }

}

static inline Hexadecimal D2H(const double x)
{
    return Hexadecimal( uint8_t(x) );
}

static inline
Proto * FFT_Mul(const WordType * const U, const size_t p,
                const WordType * const V, const size_t q,
                uint64_t * )
{
    assert(0!=U);
    assert(0!=V);
    if(p<=0)
    {
        return new Proto(0,AsCapacity);
    }
    else
    {
        if(q<=0)
        {
            return new Proto(0,AsCapacity);
        }
        else
        {
            assert(p>0);
            assert(q>0);
            const size_t   n   = p * WordSize; // U bytes
            const size_t   m   = q * WordSize; // V bytes
            const size_t   mpn = m+n;
            Batch<uint8_t> prod(mpn);

            {
                size_t       nn = 1;
                const size_t mn = Max(m,n);
                while (nn < mn)
                    nn <<= 1;
                nn <<= 1;
                Batch<double> b(nn);
                {
                    Batch<double> a(b);
                    FillBatch(a,U,p,n);
                    FillBatch(b,V,q,m);

                    //Core::Display(std::cerr << "a=", a()+1, nn, D2H) << std::endl;
                    //Core::Display(std::cerr << "b=", b()+1, nn, D2H) << std::endl;

                    FFT::Real(a(),nn,FFT::RealForward);
                    FFT::Real(b(),nn,FFT::RealForward);

                    b[1] *= a[1];
                    b[2] *= a[2];
                    for(size_t j=3;j<=nn;j+=2)
                    {
                        const double t = b[j];
                        b[j]=t*a[j]-b[j+1]*a[j+1];
                        b[j+1]=t*a[j+1]+b[j+1]*a[j];
                    }
                }
                FFT::Real(b(),nn,FFT::RealReverse);
                //Core::Display(std::cerr << "p=", b()+1, nn) << std::endl;

                static const double RX = 256.0;
                double cy=0.0;
                for(size_t j=nn;j>=1;j--)
                {
                    const double t=  (b[j]/(nn>>1)+cy+0.5);
                    cy=(unsigned long) (t/RX);
                    // std::cerr << "t=" << t << ", cy=" << cy << std::endl;
                    b[j]=t-cy*RX;
                }
                if(cy>=256)
                {
                    throw Specific::Exception(CallSign,"Invalid carry in FFT_Mul");
                }

                prod[1] = uint8_t(cy);
                for (size_t j=mpn;j>1;--j)
                    prod[j]=uint8_t(b[j-1]);
            }


            Proto *proto = new Proto(mpn,AsCapacity);

            {
                WordType *W = proto->block.entry;
                size_t    I = 0;
                size_t    k = 0;
                WordType  w = 0;
                for(size_t i=mpn;i>0;--i)
                {
                    WordType x = prod[i];
                    w |= (x<< (k<<3));
                    if(++k>=WordSize)
                    {
                        W[I++] = w;
                        k      = 0;
                        w      = 0;
                    }
                }
                if(k>0)
                    W[I] = w;
                Coerce(proto->words)  = I;
                proto->update();
            }

            return proto;
        }
    }
}

