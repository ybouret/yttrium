

inline void toRange(Patterns &patterns)
{
    assert(MINUS == curr[-1]);

    //--------------------------------------------------------------------------
    //
    // check current patterns not empty
    //
    //--------------------------------------------------------------------------
    if(patterns.size<=0)                     throw Specific::Exception(CallSign,"no character before  '-' in '%s'", expr);

    //--------------------------------------------------------------------------
    //
    // check LHS single char before '-'
    //
    //--------------------------------------------------------------------------
    assert(0!=patterns.tail);
    if(Single::UUID != patterns.tail->uuid)  throw Specific::Exception(CallSign,"no character before '-' in '%s'", expr);
    const uint8_t lhs = patterns.tail->as<Single>()->code;
    delete patterns.popTail();

    //--------------------------------------------------------------------------
    //
    // let's decipher next char
    //
    //--------------------------------------------------------------------------
    if(curr>=last) throw Specific::Exception(CallSign,"unfinished range in '%s'",expr);
    uint8_t rhs = *(curr++);

    switch(rhs)
    {
        case LBRACK:
        case RBRACK:
            throw Specific::Exception(CallSign,"invalid upper range '%s'", ASCII::Printable::Char[rhs]);

        case BACKSLASH: {
            const AutoPtr<Pattern> esc = escBand(); assert(Single::UUID==esc->uuid);
            rhs = esc->as<Single>()->code;
        } break;

        default:
            break;
    }

    //--------------------------------------------------------------------------
    //
    // done
    //
    //--------------------------------------------------------------------------
    patterns.pushTail( new Range(lhs,rhs) );

}
