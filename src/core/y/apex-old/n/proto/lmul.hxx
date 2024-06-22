
//__________________________________________________________________
//
//! long multiplication
//__________________________________________________________________
static inline
Proto * LongMul(const WordType * const a, const size_t p,
                const WordType * const b, const size_t q,
                uint64_t *ell)
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
            const size_t w       = p+q;
            Proto       *P       = new Proto(w*WordSize,AsCapacity);
            WordType    *product = P->block.entry;

            const uint64_t t = ell ? WallTime::Ticks() : 0;
            for(size_t j=0;j<q;++j)
            {
                const CoreType bj    = b[j];
                CoreType       carry = 0;
                size_t         k     = j;
                for(size_t i=0;i<p;++i,++k)
                {
                    WordType &prod = product[k];
                    carry  += CoreType(prod) + CoreType(a[i]) * bj;
                    prod    = WordType(carry&WordMask);
                    carry >>= WordBits;
                }
                assert(j+p==k);
                product[k] = WordType(carry);
            }

            Coerce(P->words)  = w;
            P->update();
            if(ell) (*ell) += WallTime::Ticks() - t;
            return P;
        }
    }
}

#if 1
static inline
Proto * LongSqr(const WordType * const a,
                const size_t           p,
                uint64_t *             ell)
{
    assert(0!=a);
    if(p<=0)
    {
        return new Proto(0,AsCapacity);
    }
    else
    {

        assert(p>0);
        const size_t w       = p+p;
        Proto       *P       = new Proto(w*WordSize,AsCapacity);
        WordType    *product = P->block.entry;

        const uint64_t t = ell ? WallTime::Ticks() : 0;
        for(size_t j=0;j<p;++j)
        {
            const CoreType bj    = a[j];
            CoreType       carry = 0;
            size_t         k     = j;
            for(size_t i=0;i<p;++i,++k)
            {
                WordType &prod = product[k];
                carry  += CoreType(prod) + CoreType(a[i]) * bj;
                prod    = WordType(carry&WordMask);
                carry >>= WordBits;
            }
            assert(j+p==k);
            product[k] = WordType(carry);
        }

        Coerce(P->words)  = w;
        P->update();
        if(ell) (*ell) += WallTime::Ticks() - t;
        return P;
    }
}
#endif

