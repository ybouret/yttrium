inline void makeJkr(Compound          &mine,
                    MakeJoker          proc,
                    const char         whom)
{
    assert(0!=proc);
    if(mine.count<=0) throw Specific::Exception(CallSign,"no expression before '%c' in '%s'",whom,expr);
    Pattern *g = mine.patterns.popTail(); assert(0!=g);
    Pattern *J = proc(g);                 assert(0!=J);
    mine << J;
}
