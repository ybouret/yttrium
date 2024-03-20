//______________________________________________________________________________
//
//
//! bytes to double
//
//______________________________________________________________________________
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

//______________________________________________________________________________
//
//
//! convert FFT product to array of bytes
//
//______________________________________________________________________________
static inline
void fft2bytes(uint8_t              prod[],
               const size_t         mpn,
               const double * const b,
               const size_t         nn,
               const double         scale)
{
    static const double  RX    = 256.0;
    static const double  IRX   = 0.00390625;
    double               carry = 0.0;
    const size_t         ntop  = mpn-1;
    const double        *addr  = &b[nn];

    for(size_t j=nn-ntop;j>0;--j)
    {
        const double  t =  floor( *(addr--)*scale+carry+0.5 );
        carry=(unsigned long) (t*IRX);
    }


    uint8_t *p = &prod[mpn];
    for(size_t j=ntop;j>0;--j)
    {
        const double  t =  floor( *(addr--)*scale+carry+0.5 );
        carry=(unsigned long) (t*IRX);
        *(p--) = static_cast<uint8_t>(t-carry*RX);
    }


    if(carry>=RX)
        throw Specific::Exception(CallSign,"Invalid carry in FFT_Mul");

    *p = static_cast<uint8_t>(carry);
}

//______________________________________________________________________________
//
//
//! convert array of bytes into words
//
//______________________________________________________________________________
static inline
void bytes2words(Pointer &             proto,
                 const uint8_t * const prod,
                 const size_t          mpn) noexcept
{
    WordType * const W = proto->block.entry;
    size_t     I = 0;
    size_t     k = 0;
    WordType   w = 0;
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

//______________________________________________________________________________
//
//
//! FFT Multiplication
//
//______________________________________________________________________________
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
            const size_t    n     = p * WordSize; // U bytes
            const size_t    m     = q * WordSize; // V bytes
            const size_t    mpn   = m+n;          // product bytes
            Pointer         proto = new Proto(mpn,AsCapacity);
            const bool      chrono = 0!=ell;
            const uint64_t  tmx    = chrono ? WallTime::Ticks() : 0;
            uint8_t *       prod   = 0;

            //__________________________________________________________________
            //
            // prepare arrays of reals
            //__________________________________________________________________
            size_t       nn = 1; //!< size for m+n
            size_t       ln = 0; //!< nn/2 = 2^ln for ScaleTable
            {
                const size_t mn = Max(m,n);
                while (nn < mn) {
                    nn <<= 1;
                    ++ln;
                }
                nn <<= 1;
                assert( (nn>>1) == (1<<ln) );
            }
            Batch<double>  workspace(nn*2);
            {
                double * const b = workspace();          // b[1..nn]
                {                                        //
                    double * const a = b+nn;             // a[1..nn]
                    prod = ((uint8_t *)&a[1])-1;         // prod[1..mpn]
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
                        const double x  = b[j];
                        const double y  = b[j1];
                        const double re = a[j];
                        const double im = a[j1];
                        b[j]  = x*re - y*im;
                        b[j1] = x*im + y*re;
                    }
                }
                FFT::ReverseReal(b,nn);
                fft2bytes(prod,mpn,b,nn,ScaleTable[ln]);
            }
            bytes2words(proto,prod,mpn);

            //__________________________________________________________________
            //
            // done
            //__________________________________________________________________
            if(chrono) (*ell) += WallTime::Ticks() - tmx;
            return proto.yield();
        }
    }
}

//______________________________________________________________________________
//
//
//! FFT Square
//
//______________________________________________________________________________
static inline
Proto * FFT_Sqr(const WordType * const U, const size_t p, uint64_t *ell)
{

    assert(0!=U);
    if(p<=0)
    {
        return new Proto(0,AsCapacity);
    }
    else
    {

        assert(p>0);
        const size_t    n      = p * WordSize; // U bytes
        const size_t    mpn    = n<<1;         // product bytes
        Pointer         proto  = new Proto(mpn,AsCapacity);
        const bool      chrono = 0!=ell;
        const uint64_t  tmx    = chrono ? WallTime::Ticks() : 0;
        uint8_t *       prod   = 0;

        //__________________________________________________________________
        //
        // prepare arrays of reals
        //__________________________________________________________________
        size_t       nn = 1; //!< size for m+n
        size_t       ln = 0; //!< nn/2 = 2^ln for ScaleTable
        {
            while (nn < n) {
                nn <<= 1;
                ++ln;
            }
            nn <<= 1;
            assert( (nn>>1) == (1<<ln) );
        }
        Batch<double>  workspace(nn*2);
        {
            double * const b = workspace();          // b[1..nn]
            {                                        //
                double * const a = b+nn;             // a[1..nn]
                prod = ((uint8_t *)&a[1])-1;         // prod[1..mpn]
                FillArray(b,U,p,n);

                //__________________________________________________________
                //
                // forward real FFT
                //__________________________________________________________
                FFT::ForwardReal(b,nn);


                //__________________________________________________________
                //
                // in-place product in b
                //__________________________________________________________
                b[1] *= b[1];
                b[2] *= b[2];
                for(size_t j=3;j<=nn;j+=2)
                {
                    const size_t j1  = j+1;
                    const double re  = b[j];
                    const double im  = b[j1];
                    b[j]  = re*re - im*im;
                    b[j1] = Twice(re*im);
                }
            }
            FFT::ReverseReal(b,nn);
            fft2bytes(prod,mpn,b,nn,ScaleTable[ln]);
        }
        bytes2words(proto,prod,mpn);

        //__________________________________________________________________
        //
        // done
        //__________________________________________________________________
        if(chrono) (*ell) += WallTime::Ticks() - tmx;
        return proto.yield();
    }
}

