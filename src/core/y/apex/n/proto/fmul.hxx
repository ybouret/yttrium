
#if 0
static inline
void FillArray_(double         * const batch,
               const WordType * const w,
               const size_t           nWords,
               const size_t           nBytes) noexcept
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
#endif

static inline
void FillArray(double         * const batch,
               const WordType * w,
               const size_t     nWords,
               const size_t     nBytes) noexcept
{
    assert(nWords*WordSize==nBytes);
    double *b = &batch[nBytes];
    for(size_t i=nWords;i>0;--i)
    {
        WordType              W = *(w++);
        const uint8_t * const B = MakeBytes::From(W);
        for(size_t k=0;k<WordSize;++k)
            *(b--) = B[k];
    }
}





static inline Hexadecimal D2H(const double x)
{
    return Hexadecimal( uint8_t(x) );
}

static inline
Proto * FFT_Mul(const WordType * const U, const size_t p,
                const WordType * const V, const size_t q,
                uint64_t *ell)
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
            const size_t   n     = p * WordSize; // U bytes
            const size_t   m     = q * WordSize; // V bytes
            const size_t   mpn   = m+n;          // product bytes
            Pointer        proto = new Proto(mpn,AsCapacity);
            Batch<uint8_t> prod(mpn);
            const bool     chrono = 0!=ell;
            const uint64_t tmx    = chrono ? WallTime::Ticks() : 0;

            {
                //______________________________________________________________
                //
                // prepare arrays of reals
                //______________________________________________________________
                size_t       nn = 1;
                const size_t mn = Max(m,n);
                while (nn < mn)
                    nn <<= 1;
                nn <<= 1;
                Batch<double> B(nn*2);
                double       *b = B(); // b[1..nn]
                {
                    //Batch<double> A(nn);
                    double *a = b+nn;  // a[1..nn]
                    FillArray(a,U,p,n);
                    FillArray(b,V,q,m);

                    //__________________________________________________________
                    //
                    // forward real FFT
                    //__________________________________________________________
                    FFT::ForwardReal(a,b,nn);

                    //__________________________________________________________
                    //
                    // in-place product in b
                    //__________________________________________________________
                    b[1] *= a[1];
                    b[2] *= a[2];
                    for(size_t j=3;j<=nn;j+=2)
                    {
                        const size_t j1 = j+1;
                        const double t  = b[j];
                        b[j]  = t*a[j]  - b[j1]*a[j1];
                        b[j1] = t*a[j1] + b[j1]*a[j];
                    }
                }
                FFT::ReverseReal(b,nn);

                //______________________________________________________________
                //
                // in place product as array of bytes
                //______________________________________________________________
                static const double RX    = 256.0;
                double              carry = 0.0;
                const double        scale = 1.0/(nn>>1);
                for(size_t j=nn;j>=1;--j)
                {
                    double      * const f = &b[j];
                    const double        t =  floor( (*f)*scale+carry+0.5 );
                    carry=(unsigned long) (t/RX);
                    *(uint8_t *)f = static_cast<uint8_t>(t-carry*RX);
                }

                if(carry>=RX)
                    throw Specific::Exception(CallSign,"Invalid carry in FFT_Mul");

                prod[1] = static_cast<uint8_t>(carry);
                {
                    const double *f = &b[mpn];
                    for (size_t j=mpn;j>1;--j)
                        prod[j] = *(const uint8_t *)(--f);
                }
            }


            //__________________________________________________________________
            //
            // transform array of bytes into array of words
            //__________________________________________________________________
            {
                WordType *W = proto->block.entry;
                size_t    I = 0;
                size_t    k = 0;
                WordType  w = 0;
                for(size_t i=mpn;i>0;--i)
                {
                    static const unsigned kshift[4] = { 0, 8, 16, 24 };
                    w |= ( prod[i] << kshift[k] );
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

            //__________________________________________________________________
            //
            // done
            //__________________________________________________________________
            if(chrono) (*ell) += WallTime::Ticks() - tmx;
            return proto.yield();
        }
    }
}

