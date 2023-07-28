
//__________________________________________________________________
//
//! update metrics from CURRENT number of words
//__________________________________________________________________
inline void update() noexcept
{
    while(words>0)
    {
        const size_t    msi = words-1;
        const WordType &msw = block.entry[msi];
        if(msw<=0) { Coerce(words) = msi; continue; }
        assert(words>0);
        Coerce(nbits) = BitCount::For(msw) + msi * WordBits;
        Coerce(bytes) = BitsToBytes(nbits);
        return;
    }
    assert(0==words);
    Coerce(bytes) = 0;
    Coerce(nbits) = 0;
}

//__________________________________________________________________
//
//! Least Significant 64 bits
//__________________________________________________________________
inline uint64_t ls64() const noexcept
{
    uint64_t qw = 0;
    for(size_t i=words;i>0;)
        (qw <<= WordBits) |= block.entry[--i];
    return qw;
}

//__________________________________________________________________
//
//! display status
//__________________________________________________________________
inline void display() const
{
    std::cerr << "<APEX " << CoreBits << "-" << WordBits;
    std::cerr << " bytes="  << std::setw(3) << bytes;
    std::cerr << ", words=" << std::setw(3) << words;
    std::cerr << ", nbits=" << std::setw(5) << nbits ;
    std::cerr << ">";
    std::cerr << " [";
    for(size_t i=0;i<words;++i) std::cerr << ' ' << Hexadecimal(block.entry[i]);
    std::cerr << " ]" << std::endl;
}

//__________________________________________________________________
//
//! clean extraneous words
//__________________________________________________________________
inline void ztrim() noexcept { memset(block.entry+words,0,(block.words-words)*WordSize); }

//__________________________________________________________________
//
//! get byte[0..bytes-1]
//__________________________________________________________________
inline uint8_t getByte(const size_t i) const noexcept
{
    assert(i<bytes);
    WordType w = block.entry[i/WordSize];
    size_t   r = i%WordSize;
    while(r-- > 0) w = UnsignedInt<WordSize>::SHR8(w);
    return uint8_t(w);
}

//__________________________________________________________________
//
//! Hexadecimal printing
//__________________________________________________________________
inline std::ostream & printHex(std::ostream &os) const
{
    if(bytes<=0) { os << '0'; return os; }

    bool   first = true;
    size_t i     = bytes;
    while(i>0)
    {
        const uint8_t b = getByte(--i);
        const uint8_t l = b>>4;
        if(first)
        {
            assert(b>0);
            if(l>0) os << Hexadecimal::Upper[l];
            first = false;
        }
        else
        {
            os << Hexadecimal::Upper[l];
        }
        os << Hexadecimal::Upper[b&0xf];
    }
    return os;
}

//__________________________________________________________________
//
//! try to steal proto
//__________________________________________________________________
inline bool couldSteal(const Proto &other) noexcept
{
    if(other.words<=block.words)
    {
        Coerce(bytes) = other.bytes;
        Coerce(words) = other.words;
        Coerce(nbits) = other.nbits;
        memcpy(block.entry,other.block.entry,words*WordSize);
        ztrim();
        assert(Check("couldSteal(other)"));
        return true;
    }
    else
        return false;
}

//__________________________________________________________________
//
//! load 64 bits
//__________________________________________________________________
inline void ld64(const uint64_t qword) noexcept
{
    Coerce(nbits) = BitCount::For(qword);
    Coerce(bytes) = BitsToBytes(nbits);
    Coerce(words) = Splitter::BytesToWords(bytes);
    Splitter::DoSplit(block.entry,words,qword);
    ztrim();
    assert(Check("ld64(qword)"));
}
