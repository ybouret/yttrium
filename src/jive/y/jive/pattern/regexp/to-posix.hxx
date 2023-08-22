
Pattern *toPosix()
{
    // Look for ':]'
    const char * const ini = curr;
    const char * const end = strchr(ini,COLON);
    const char * const nxt = end+1;
    if(0==end || end>=last||nxt>=last)  throw Specific::Exception(CallSign,"unfinished posix after '%s'", ini);
    if(RBRACK!= *nxt)                   throw Specific::Exception(CallSign,"invalid posix '%s'",ini);

    // deduce name
    const String name(ini,end-ini);

    // look for it in posixDict
    Pattern *q = posixDict.create(name);
    if(!q) throw Specific::Exception(CallSign,"unknown [:%s:]",name());

    // update satus and return
    curr = nxt+1;
    return q;
}
