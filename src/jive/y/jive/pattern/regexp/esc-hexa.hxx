
Pattern * escHexa()
{
    assert('x'==curr[-1]);
    if(curr>=last) throw Specific::Exception(CallSign,"missing first hexadecimal character in '%s'",expr);
    const uint8_t hiChar = *(curr++);
    const int     hiByte = Hexadecimal::ToDecimal(hiChar);
    if(hiByte<0)
        throw Specific::Exception(CallSign,"invalid first hexadecimal '%s' in '%s'", ASCII::Printable::Char[hiChar], expr);

    if(curr>=last) throw Specific::Exception(CallSign,"missing second hexadecimal character in '%s'",expr);
    const uint8_t loChar = *(curr++);
    const int     loByte = Hexadecimal::ToDecimal(loChar);
    if(loByte<0)
        throw Specific::Exception(CallSign,"invalid second hexadecimal '%s' in '%s'", ASCII::Printable::Char[loChar], expr);

    const uint8_t code = uint8_t(hiByte*16+loByte);
    return new Single(code);
}
