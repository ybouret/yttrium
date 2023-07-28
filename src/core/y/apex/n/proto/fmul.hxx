
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
            const size_t   w = p+q;          // result #words
            const size_t   n = w * WordSize; // result #bytes
            Batch<cplx>    fft1(n);          // as many cplx
            Batch<cplx>    fft2(fft1);  assert(fft2.count==fft1.count);
            const size_t   N = fft1.count;

            std::cerr << "re :";
            FillRe(fft1(),a,p); std::cerr << std::endl;
            std::cerr << "im :";
            FillIm(fft1(),b,q); std::cerr << std::endl;

            Core::Display(std::cerr << "pack=", fft1(), N) << std::endl;
            cplx::Type *f1 = (&fft1()->re) - 1;
            cplx::Type *f2 = (&fft2()->re) - 1;
            FFT::Forward(f1,N);
            FFT::Expand(f1,f2,N);
            Core::Display(std::cerr << "fft1=", fft1(), N) << std::endl;
            Core::Display(std::cerr << "fft2=", fft2(), N) << std::endl;
            for(size_t i=0;i<N;++i)
            {
                fft1[i] *= fft2[i];
            }
            FFT::Reverse(f1,N);
            for(size_t i=N*2;i>0;--i) f1[i] /= N;
            Core::Display(std::cerr << "fft1=", fft1(), N) << std::endl;

            
            return 0;
        }
    }
}

