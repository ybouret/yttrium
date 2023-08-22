
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
            case LPAREN:
                ++deep;
                *p << subExpr();
                break;


            case RPAREN:
                if(deep--<=0)
                    throw Specific::Exception(RegExpCompiler::CallSign,"extraneous '%c' in '%s'", RPAREN, expr);
                goto FINISH;

            default:
                p->add(c);
        }
    }


    // check at this point
FINISH:
    if(p->count<=0) throw Specific::Exception(RegExpCompiler::CallSign,"empty sub-expression in '%s'",expr);
    return Pattern::Optimize( p.yield() );
}

