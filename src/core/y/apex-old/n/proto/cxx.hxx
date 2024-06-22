
//__________________________________________________________________
//
//
// C++
//
//__________________________________________________________________

//__________________________________________________________________
//
//! debugging helper
//__________________________________________________________________
inline bool Check(const char *fn) const
{
    assert(0!=fn);
    if( !CheckMetrics(fn,nbits,bytes,words,WordSize,block.words) )
        return false;

    if( ! Memory::OutOfReach::Are0(block.entry+words,(block.words-words)*WordSize ) )
    {
        std::cerr << fn << " dirty trailing bytes!" << std::endl;
        return false;
    }

    return true;
}

//__________________________________________________________________
//
//! create a Proto with a given minimal BYTES capacity
//__________________________________________________________________
inline explicit Proto(const size_t      numBytes,
                      const AsCapacity_ &)  :
Nexus::Proto(),
nbits(0),
bytes(0),
words(0),
block(numBytes)
{
    Y_STATIC_CHECK(WordSize<CoreSize,InvalidMetrics);
}

//__________________________________________________________________
//
//! create a Proto with one byte
//__________________________________________________________________
inline explicit Proto(const uint8_t b,
                      const AsByte_ &)  :
Nexus::Proto(),
nbits( BitCount::For(b) ),
bytes( nbits > 0 ? 1 : 0),
words( bytes > 0 ? 1 : 0),
block(1)
{
    block.entry[0] = b;
    assert(Check("Proto(AsByte)"));
}


//__________________________________________________________________
//
//! create a Proto with a given uint64_t
//__________________________________________________________________
inline explicit Proto(const uint64_t qword) :
Nexus::Proto(),
nbits( BitCount::For(qword)          ),
bytes( BitsToBytes(nbits)            ),
words( Splitter::BytesToWords(bytes) ),
block( sizeof(qword)                 )
{
    assert(block.words>=Splitter::MaxWords);
    Splitter::DoSplit(block.entry,words,qword);
    assert(Check("Proto(uint64_t)"));
}

//__________________________________________________________________
//
//! copy constructor
//__________________________________________________________________
inline Proto(const Proto &proto) :
Nexus::Proto(),
nbits( proto.nbits ),
bytes( proto.bytes ),
words( proto.words ),
block( proto.block )
{
    memcpy(block.entry,proto.block.entry,words*WordSize);
    assert(Check("Proto(copy)"));
}

//__________________________________________________________________
//
//! create a random Proto with exactly userBits
//__________________________________________________________________
inline Proto(const size_t userBits, Random::Bits &ran) :
Nexus::Proto(),
nbits( userBits ),
bytes( BitsToBytes(nbits) ),
words( Splitter::BytesToWords(bytes) ),
block( bytes )
{
    if(words>0)
    {
        const  size_t msi = words-1;
        size_t        num = nbits;

        for(size_t i=0;i<msi;++i)
        {
            block.entry[i] = ran.to<WordType>();
            assert(num>WordBits);
            num -= WordBits;
        }
        assert(num>0);
        block.entry[msi] = ran.to<WordType>(unsigned(num));
    }
    update();
    assert(nbits==userBits);
    assert(Check("Proto(rand)"));
}

//__________________________________________________________________
//
//! create a Proto from W[N], then update
//__________________________________________________________________
inline Proto(const WordType *W, const size_t    N) :
Nexus::Proto(), nbits(0), bytes(N*WordSize), words(N), block(bytes)
{
    assert(0!=W);
    memcpy(block.entry,W,bytes);
    update();
    assert(Check("Proto(W,N)"));
}


inline virtual ~Proto() noexcept {}
