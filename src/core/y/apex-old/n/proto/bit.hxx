
//__________________________________________________________________
//
//
// Bitwise ops
//
//__________________________________________________________________

//__________________________________________________________________
//
//! construct 2^sh
//__________________________________________________________________
inline Proto(const size_t sh, const AsShift_ &) :
Nexus::Proto(),
nbits( sh+1 ),
bytes( BitsToBytes(nbits)            ),
words( Splitter::BytesToWords(bytes) ),
block(bytes)
{
    assert(bytes>0);
    assert(words>0);

    const size_t  msi = words-1; assert(nbits>msi*WordBits);
    WordType     &msw = block.entry[msi];
    const size_t  msb = nbits - msi * WordBits; assert(msb>0); assert(msb<=WordBits);
    msw = WordType(1) << (msb-1);

    assert(Check("Proto(AsShift)"));
}

//__________________________________________________________________
//
//! this >>= 1
//__________________________________________________________________
inline void shr() noexcept
{
    if(words>0)
    {
        WordType    *w   = block.entry;
        const size_t msi = words-1;
        for(size_t i=0;i<msi;++i)
        {
            WordType b1 = w[i+1] & 0x01;
            (w[i] >>=1 ) |= (b1<<(WordBits-1));
        }
        w[msi] >>= 1;
        update();
    }
    assert(Check("shr"));
    }
