
template <> ZBis<real_t>::  ZBis() noexcept : ZFind<real_t>() {}
template <> ZBis<real_t>:: ~ZBis() noexcept {}


template <>
const char * ZBis<real_t>:: callSign() const noexcept { return CallSign; }

template <>
void ZBis<real_t>:: lookup(Triplet<real_t> &x, Triplet<real_t> &f, FunctionType &F)
{

    Handle xp = {0,0}, fp = {0,0};
    if( found(xp,fp,x,f,F)) return;


    real_t width = Fabs<real_t>::Of(x.c-x.a);

    while(true)
    {
        switch( Sign::Of( f.b = F( x.b = half*(x.a+x.c) ) ) )
        {
            case __Zero__: return; // early return
            case Negative: *fp.neg = f.b; *xp.neg= x.b; break;
            case Positive: *fp.pos = f.b; *xp.pos= x.b; break;
        }
        const real_t newWidth = Fabs<real_t>::Of(x.c-x.a);
        if(newWidth>=width)
            return;
        width = newWidth;
    }

}
