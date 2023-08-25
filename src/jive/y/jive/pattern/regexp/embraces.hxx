
inline void embraces(Patterns &patterns)
{
    assert(LBRACE==curr[-1]);
    if(curr>=last) throw Specific::Exception(CallSign,"unfinished braces in '%s'", expr);

    const char * const ini = curr;
    const char * const end = strchr(ini,RBRACE);
    if(!end) throw Specific::Exception(CallSign,"missing closing brace in '%s'",expr);

    String content(ini,end-ini);
    std::cerr << "Brace content: '" << content << "'" << std::endl;

    curr = end+1;

}
