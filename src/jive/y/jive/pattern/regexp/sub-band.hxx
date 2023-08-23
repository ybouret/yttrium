Pattern *subBand()
{
    if(curr>=last) throw Specific::Exception(CallSign,"unfinished band in '%s'",expr);

    AutoPtr<Compound> p = 0;

    //--------------------------------------------------------------------------
    //
    // starting according to first char
    //
    //--------------------------------------------------------------------------
    switch(curr[0])
    {
        case CARET:
            p = new None();
            ++curr;
            if(curr<last && curr[0] == MINUS)
            {
                p->add(MINUS);
                ++curr;
            }
            break;

        case MINUS:
            p = new Or();
            p->add(MINUS);
            ++curr;
            break;

        case COLON:
            ++curr;
            return toPosix();

        default:
            p = new Or();
            break;
    }

    //--------------------------------------------------------------------------
    //
    // Loop over next chars
    //
    //--------------------------------------------------------------------------
    while(curr<last)
    {
        const char c = *(curr++);
        switch(c)
        {
                //--------------------------------------------------------------
                // ending
                //--------------------------------------------------------------
            case RBRACK:
                goto FINISH;

                //--------------------------------------------------------------
                // recursive
                //--------------------------------------------------------------
            case LBRACK:
                *p << subBand();
                break;

                //--------------------------------------------------------------
                // escape
                //--------------------------------------------------------------
            case BACKSLASH:
                * p << escBand();
                break;

                //--------------------------------------------------------------
                // Range detection
                //--------------------------------------------------------------
            case MINUS:
                makeRange(p->patterns);
                break;

                //--------------------------------------------------------------
                // default
                //--------------------------------------------------------------
            default:
                p->add(c);
        }

    }

    throw Specific::Exception(CallSign,"unfinished band in '%s'",expr);

FINISH:
    return Pattern::Optimize( p.yield() );
}


inline void makeRange(Patterns &patterns)
{
    assert(MINUS == curr[-1]);
    std::cerr << "Now in makeRange" << std::endl;


    // check patterns not empty
    if(patterns.size<=0)                     throw Specific::Exception(CallSign,"no character before  '-' in '%s'", expr);

    // check single char before '-'
    assert(0!=patterns.tail);
    if(Single::UUID != patterns.tail->uuid)  throw Specific::Exception(CallSign,"no SINGLE char before '-' in '%s'", expr);

    throw Exception("makeRange not implemented");
}
