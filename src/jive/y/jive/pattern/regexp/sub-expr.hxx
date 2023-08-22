
inline Pattern *subExpr()
{
    // create new 'And'
    AutoPtr<Compound> p = new And();

    //--------------------------------------------------------------------------
    //
    // loop over input
    //
    //--------------------------------------------------------------------------
    while(curr<last)
    {
        const char c = *(curr++);
        switch(c)
        {
                //--------------------------------------------------------------
                //
                //  Grouping
                //
                //---------------------------------------------------------------
            case LPAREN:
                ++deep;
                *p << subExpr();
                break;

            case RPAREN:
                if(deep--<=0)
                    throw Specific::Exception(CallSign,"extraneous '%c' in '%s'", RPAREN, expr);
                goto FINISH;


                //--------------------------------------------------------------
                //
                //  Alternation
                //
                //--------------------------------------------------------------
            case ALT: {
                AutoPtr<Compound> alt = new Or();
                *alt << p.yield(); assert(p.isEmpty());
                *alt << subExpr();
                p = alt;
            } goto FINISH;

                //--------------------------------------------------------------
                //
                //  Jokers
                //
                //--------------------------------------------------------------
            case '+': assert(p.isValid()); makeJkr(*p,Repeating::OneOrMore,  '+'); break;
            case '*': assert(p.isValid()); makeJkr(*p,Repeating::ZeroOrMore, '*'); break;
            case '?': assert(p.isValid()); makeJkr(*p,Optional::From,        '?'); break;

                //--------------------------------------------------------------
                //
                //  escape sequence
                //
                //--------------------------------------------------------------
            case BACKSLASH:
                *p << escExpr();
                break;

            default:
                p->add(c);
        }
    }


    // check at this point
FINISH:
    if(p->count<=0) throw Specific::Exception(CallSign,"empty sub-expression in '%s'",expr);
    return Pattern::Optimize( p.yield() );
}

