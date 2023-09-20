
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





static inline
Proto * FFT_Mul(const WordType * const a, const size_t p,
                const WordType * const b, const size_t q,
                uint64_t * )
{
    assert(0!=a);
    assert(0!=b);
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



            return new Proto(0,AsCapacity);
        }
    }
}

