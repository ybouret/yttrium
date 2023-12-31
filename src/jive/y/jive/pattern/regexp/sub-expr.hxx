
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
            case LBRACE:
                embraces(p->patterns);
                break;

            case AMPERSAND:
                ignoreCase(p->patterns);
                break;

                //--------------------------------------------------------------
                //
                //  Band
                //
                //--------------------------------------------------------------
            case LBRACK:
                *p << subBand();
                break;

            case '.':
                *p << posix::dot();
                break;

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
    if(p->count<=0) throw Specific::Exception(CallSign,"empty expression in '%s'",expr);
    return p.yield();
    //return Pattern::Optimize( p.yield() );
}

inline void ignoreCase(Patterns &patterns)
{
    //std::cerr << "ignoring case..." << std::endl;
    if( patterns.size <=0 ) throw Specific::Exception(CallSign,"no pattern before '&' in '%s'",expr);
    patterns.pushTail( Pattern::IgnoreCase(patterns.popTail()) );
}
