
//__________________________________________________________________
//
//! Addition
/**
 \param lhs left words
 \param lnw left num words
 \param rhs right words
 \param rnw right num words
 \param ell ellapsed ticks in computation
 */
//__________________________________________________________________
static inline
Proto * Add(const WordType * const lhs,
            const size_t           lnw,
            const WordType * const rhs,
            const size_t           rnw,
            uint64_t *             ell = 0)
{
    assert(0!=lhs);
    assert(0!=rhs);
    if(lnw<=0)
    {
        if(rnw<=0)
            //------------------------------------------------------
            // 0 + 0
            //------------------------------------------------------
            return new Proto(0,AsCapacity);
        else
            //------------------------------------------------------
            // 0 + rhs = rhs
            //------------------------------------------------------
            return new Proto(rhs,rnw);
    }
    else
    {
        assert(lnw>0);
        if(rnw<=0)
        {
            //------------------------------------------------------
            // lhs + 0 = lhs
            //------------------------------------------------------
            return new Proto(lhs,lnw);
        }
        else
        {
            //------------------------------------------------------
            // order a >= b
            //------------------------------------------------------
            const WordType *a  = lhs; assert(0!=a);
            const size_t    na = lnw; assert(na>0);
            const WordType *b  = rhs; assert(0!=b);
            const size_t    nb = rnw; assert(nb>0);
            if(na<nb) { Swap(a,b); CoerceSwap(na,nb); }
            assert(na>=nb);

            //------------------------------------------------------
            // acquire resources
            //------------------------------------------------------
            const size_t    ns = na+1;
            Proto          *S  = new Proto(ns << WordLog2,AsCapacity); assert(S->block.words>=ns);
            WordType       *s  = S->block.entry;
            const uint64_t  t  = ell ? WallTime::Ticks() : 0;
            {
                //--------------------------------------------------
                // initialize algorithm
                //--------------------------------------------------
                CoreType sum = CoreType(a[0]) + CoreType(b[0]);
                s[0]         = static_cast<WordType>(sum);

                //--------------------------------------------------
                // common word(s)
                //--------------------------------------------------
                for(size_t i=1;i<nb;++i)
                {
                    sum >>= WordBits;
                    sum += CoreType(a[i]) + CoreType(b[i]);
                    s[i] = static_cast<WordType>(sum);
                }

                //--------------------------------------------------
                // extra word(s)
                //--------------------------------------------------
                for(size_t i=nb;i<na;++i)
                {
                    sum >>= WordBits;
                    sum += CoreType(a[i]);
                    s[i] = static_cast<WordType>(sum);
                }

                //--------------------------------------------------
                // final word
                //--------------------------------------------------
                sum >>= WordBits;
                s[na] = static_cast<WordType>(sum);
            }

            //------------------------------------------------------
            // update resources
            //------------------------------------------------------
            Coerce(S->words) = ns;
            S->update();
            if(ell) (*ell) += WallTime::Ticks() - t;
            return S;
        }

    }
}

//__________________________________________________________________
//
//! Addition for two Proto
//__________________________________________________________________
static inline Proto * Add(const Proto &lhs, const Proto &rhs, uint64_t    *ell=0)
{
    return Add(lhs.block.entry,lhs.words,rhs.block.entry,rhs.words,ell);
}

//__________________________________________________________________
//
//! Addition for Proto + uint64_t
//__________________________________________________________________
static inline Proto * Add(const Proto &lhs, const uint64_t &rhs)
{
    const Splitter alias(rhs);
    return Add(lhs.block.entry,lhs.words,alias.w,alias.n);
}

//__________________________________________________________________
//
//! Add one
//__________________________________________________________________
static inline Proto *Add1(const Proto &lhs)
{
    static const WordType One(1);
    return Add(lhs.block.entry,lhs.words,&One,1);
    }
