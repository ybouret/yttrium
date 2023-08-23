

inline void toRange(Patterns &patterns)
{
    assert(MINUS == curr[-1]);
    std::cerr << "Now in makeRange" << std::endl;


    // check patterns not empty
    if(patterns.size<=0)                     throw Specific::Exception(CallSign,"no character before  '-' in '%s'", expr);

    // check single char before '-'
    assert(0!=patterns.tail);
    if(Single::UUID != patterns.tail->uuid)  throw Specific::Exception(CallSign,"no SINGLE char before '-' in '%s'", expr);


    // let's decipher next char
    if(curr>=last) throw Specific::Exception(CallSign,"unfinished RANGE in '%s'",expr);

    const char rhs = *(curr++);
    

    throw Exception("makeRange not implemented");
}
