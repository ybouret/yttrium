
static inline
void FillArray(double         * const batch,
               const WordType * const w,
               const size_t           nWords,
               const size_t           nBytes) noexcept
{
    assert(nWords*WordSize==nBytes);
    double *top = &batch[nBytes];
    for(size_t i=0;i<nWords;++i)
        MakeBytes::ReversePush(top,w[i]);
}

#if 0
static inline
void FillArray_(double         * const batch,
               const WordType * w,
               const size_t     nWords,
               const size_t     nBytes) noexcept
{
    assert(nWords*WordSize==nBytes);
    double *b = &batch[nBytes];
    for(size_t i=nWords;i>0;--i)
    {
        WordType              W = *(w++);
        const uint8_t * const B = MakeBytes::BE(W);
        for(size_t k=0;k<WordSize;++k)
            *(b--) = B[k];
    }
}
#endif


static inline Hexadecimal D2H(const double x) noexcept
{
    return Hexadecimal( uint8_t(x) );
}

static inline
Proto * FFT_Mul(const WordType * const U, const size_t p,
                const WordType * const V, const size_t q,
                uint64_t *ell)
{
    static const double ScaleTable[64] =
    {
        1.0/double(Y_U64(0x0000000000000001)),
        1.0/double(Y_U64(0x0000000000000002)),
        1.0/double(Y_U64(0x0000000000000004)),
        1.0/double(Y_U64(0x0000000000000008)),
        1.0/double(Y_U64(0x0000000000000010)),
        1.0/double(Y_U64(0x0000000000000020)),
        1.0/double(Y_U64(0x0000000000000040)),
        1.0/double(Y_U64(0x0000000000000080)),
        1.0/double(Y_U64(0x0000000000000100)),
        1.0/double(Y_U64(0x0000000000000200)),
        1.0/double(Y_U64(0x0000000000000400)),
        1.0/double(Y_U64(0x0000000000000800)),
        1.0/double(Y_U64(0x0000000000001000)),
        1.0/double(Y_U64(0x0000000000002000)),
        1.0/double(Y_U64(0x0000000000004000)),
        1.0/double(Y_U64(0x0000000000008000)),
        1.0/double(Y_U64(0x0000000000010000)),
        1.0/double(Y_U64(0x0000000000020000)),
        1.0/double(Y_U64(0x0000000000040000)),
        1.0/double(Y_U64(0x0000000000080000)),
        1.0/double(Y_U64(0x0000000000100000)),
        1.0/double(Y_U64(0x0000000000200000)),
        1.0/double(Y_U64(0x0000000000400000)),
        1.0/double(Y_U64(0x0000000000800000)),
        1.0/double(Y_U64(0x0000000001000000)),
        1.0/double(Y_U64(0x0000000002000000)),
        1.0/double(Y_U64(0x0000000004000000)),
        1.0/double(Y_U64(0x0000000008000000)),
        1.0/double(Y_U64(0x0000000010000000)),
        1.0/double(Y_U64(0x0000000020000000)),
        1.0/double(Y_U64(0x0000000040000000)),
        1.0/double(Y_U64(0x0000000080000000)),
        1.0/double(Y_U64(0x0000000100000000)),
        1.0/double(Y_U64(0x0000000200000000)),
        1.0/double(Y_U64(0x0000000400000000)),
        1.0/double(Y_U64(0x0000000800000000)),
        1.0/double(Y_U64(0x0000001000000000)),
        1.0/double(Y_U64(0x0000002000000000)),
        1.0/double(Y_U64(0x0000004000000000)),
        1.0/double(Y_U64(0x0000008000000000)),
        1.0/double(Y_U64(0x0000010000000000)),
        1.0/double(Y_U64(0x0000020000000000)),
        1.0/double(Y_U64(0x0000040000000000)),
        1.0/double(Y_U64(0x0000080000000000)),
        1.0/double(Y_U64(0x0000100000000000)),
        1.0/double(Y_U64(0x0000200000000000)),
        1.0/double(Y_U64(0x0000400000000000)),
        1.0/double(Y_U64(0x0000800000000000)),
        1.0/double(Y_U64(0x0001000000000000)),
        1.0/double(Y_U64(0x0002000000000000)),
        1.0/double(Y_U64(0x0004000000000000)),
        1.0/double(Y_U64(0x0008000000000000)),
        1.0/double(Y_U64(0x0010000000000000)),
        1.0/double(Y_U64(0x0020000000000000)),
        1.0/double(Y_U64(0x0040000000000000)),
        1.0/double(Y_U64(0x0080000000000000)),
        1.0/double(Y_U64(0x0100000000000000)),
        1.0/double(Y_U64(0x0200000000000000)),
        1.0/double(Y_U64(0x0400000000000000)),
        1.0/double(Y_U64(0x0800000000000000)),
        1.0/double(Y_U64(0x1000000000000000)),
        1.0/double(Y_U64(0x2000000000000000)),
        1.0/double(Y_U64(0x4000000000000000)),
        1.0/double(Y_U64(0x8000000000000000))
    };
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
                size_t       ln = 0;
                {
                    const size_t mn = Max(m,n);
                    while (nn < mn)
                    {
                        nn <<= 1;
                        ++ln;
                    }
                    nn <<= 1;
                    assert( (nn>>1) == (1<<ln) );
                }
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
                static const double IRX   = 0.00390625;
                double              carry = 0.0;
                const double        scale = ScaleTable[ln];
                for(size_t j=nn;j>=1;--j)
                {
                    double      * const f = &b[j];
                    const double        t =  floor( (*f)*scale+carry+0.5 );
                    carry=(unsigned long) (t*IRX);
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

