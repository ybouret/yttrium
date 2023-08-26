
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
    }


    throw Specific::Exception(CallSign,"invalid char '%s' within in braces in '%s'",ASCII::Printable::Char[choice],expr);

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


inline void insertJoker(Patterns &patterns, const String &name)
{
    if(patterns.size<=0) throw Specific::Exception(CallSign,"no pattern before braces in '%s'", expr);
    
}
