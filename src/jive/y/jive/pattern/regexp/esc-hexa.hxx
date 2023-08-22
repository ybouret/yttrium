

inline uint8_t getHexa(const char *which)
{
    assert(0!=which);
    if(curr>=last) throw Specific::Exception(CallSign,"missing %s hexadecimal character in '%s'",which,expr);
    const uint8_t c = *(curr++);
    const int     h = Hexadecimal::ToDecimal(c);
    if(h<0)
        throw Specific::Exception(CallSign,"invalid %s hexadecimal '%s' in '%s'", which, ASCII::Printable::Char[c], expr);

    return uint8_t(h);
}

inline Pattern * escHexa()
{
    assert('x'==curr[-1]);
    const uint8_t hi = getHexa("first");
    const uint8_t lo = getHexa("second");
    return new Single( (hi<<4) | lo );
}
