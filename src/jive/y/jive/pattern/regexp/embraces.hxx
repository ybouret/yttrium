
inline void embraces(Patterns &patterns)
{
    assert(LBRACE==curr[-1]);

    const char * const ini = curr;               if(curr>=last)        throw Specific::Exception(CallSign,"unfinished braces in '%s'", expr);
    const char * const end = strchr(ini,RBRACE); if(!end)              throw Specific::Exception(CallSign,"missing closing brace in '%s'",expr);
    const String       content(ini,end-ini);     if(content.size()<=0) throw Specific::Exception(CallSign,"empty braces in '%s'",expr);
    const uint8_t      choice = content[1];

    if(isalpha(choice))
    {
        // assuming alias
        insertAlias(patterns,content);
        goto DONE;
    }
    else
    {
        // assuming joker
        insertJoker(patterns,content);
        goto DONE;
    }


    throw Specific::Exception(CallSign,"invalid first char '%s' within in braces in '%s'",ASCII::Printable::Char[choice],expr);

DONE:
    curr = end+1;

}

inline void insertAlias(Patterns &patterns, const String &name)
{
    if(0==userDictP) throw Specific::Exception(CallSign,"no dictionary for '%s' in '%s'", name(), expr);
    Pattern *alias = userDictP->create(name);
    if(0 == alias)   throw Specific::Exception(CallSign,"no alias '%s' in '%s'", name(), expr);
    patterns.pushTail(alias);
}


inline void insertJoker(Patterns &patterns, const String &param)
{
    if(patterns.size<=0) throw Specific::Exception(CallSign,"no pattern before braces in '%s'", expr);
    
    const char * const ini = param.c_str();
    const char * const sep = strchr( param.c_str(), ',' );
    if(sep)
    {
        const String lhs(ini,sep-ini);
        const String rhs(sep+1);
        if(rhs.size()<=0)
        {
            const size_t atLeast = ASCII::Convert::To<size_t>(lhs,"pattern at least count");
            patterns.pushTail( Repeating::Make(atLeast,patterns.popTail()) );
        }
        else
        {
            const size_t minCount = ASCII::Convert::To<size_t>(lhs,"pattern min count");
            const size_t maxCount = ASCII::Convert::To<size_t>(rhs,"pattern max count");
            patterns.pushTail( Counting::Make(minCount,maxCount, patterns.popTail()) );
        }

    }
    else
    {
        const size_t n = ASCII::Convert::To<size_t>(param,"pattern count");
        patterns.pushTail( Counting::Make(n, n, patterns.popTail()) );
    }



}
