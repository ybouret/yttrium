
//__________________________________________________________________
//
//! Subtraction
/**
 \param lhs left words
 \param lnw left num words
 \param rhs right words
 \param rnw right num words
 */
//__________________________________________________________________
static inline
Proto * Sub(const WordType * const lhs,
            const size_t           lnw,
            const WordType * const rhs,
            const size_t           rnw)
{
    assert(0!=lhs);
    assert(0!=rhs);
    if(lnw<=0)
    {
        if(rnw<=0)
        {
            //------------------------------------------------------
            // 0 - 0
            //------------------------------------------------------
            return new Proto(0,AsCapacity);
        }
        else
        {
            //------------------------------------------------------
            // 0 - (>0)
            //------------------------------------------------------
            throw Specific::Exception(CallSign,"subtract positive from 0");
        }
    }
    else
    {
        assert(lnw>0);
        if(rnw<=0)
        {
            //------------------------------------------------------
            // lhs  - 0
            //------------------------------------------------------
            return new Proto(lhs,lnw);
        }
        else
        {
            assert(rnw>0);
            //------------------------------------------------------
            // lhs  - rhs, generic case
            //------------------------------------------------------
            if(rnw>lnw) throw Specific::Exception(CallSign,"rhs>lhs level-1");

            Proto    *D = new Proto(lnw*WordSize,AsCapacity);
            WordType *d = D->block.entry;
            assert(lnw>=rnw);

            {
                CIntType carry = 0;
                for(size_t i=0;i<rnw;++i)
                {
                    const CIntType del = CIntType(lhs[i]) - CIntType(rhs[i]) - carry;
                    if(del<0)
                    {
                        d[i]  = WordType(del+Radix);
                        carry = 1;
                    }
                    else
                    {
                        d[i]  = WordType(del);
                        carry = 0;
                    }
                }

                for(size_t i=rnw;i<lnw;++i)
                {
                    CIntType del = CIntType(lhs[i]) - carry;
                    if(del<0)
                    {
                        d[i]  = WordType(del+Radix);
                        carry = 1;
                    }
                    else
                    {
                        d[i]  = WordType(del);
                        carry = 0;
                    }
                }

                if(0!=carry)
                {
                    delete D;
                    throw Specific::Exception(CallSign,"rhs>lhs level-2");
                }
            }

            Coerce(D->words) = lnw;
            D->update();


            return D;
        }
    }

}

//__________________________________________________________________
//
//! Subtraction for Proto - Proto
//__________________________________________________________________
static inline Proto * Sub(const Proto &lhs, const Proto &rhs)
{
    return Sub(lhs.block.entry,lhs.words,rhs.block.entry,rhs.words);
}

//__________________________________________________________________
//
//! Subtraction for Proto - uint64_t
//__________________________________________________________________
static inline Proto * Sub(const Proto &lhs, const uint64_t &rhs)
{
    const Splitter alias(rhs);
    return Sub(lhs.block.entry,lhs.words,alias.w,alias.n);
}

//__________________________________________________________________
//
//! Subtraction for uint64_t - proto
//__________________________________________________________________
static inline Proto * Sub(const uint64_t lhs, const Proto   &rhs)
{
    const Splitter alias(lhs);
    return Sub(alias.w,alias.n,rhs.block.entry,rhs.words);
}

//__________________________________________________________________
//
//! Subtract one
//__________________________________________________________________
static inline Proto *Sub1(const Proto &lhs)
{
    static const WordType One(1);
    return Sub(lhs.block.entry,lhs.words,&One,1);
    }
