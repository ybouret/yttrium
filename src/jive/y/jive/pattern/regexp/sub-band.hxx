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

