
template <> ZBis<real_t>::  ZBis() noexcept : ZFind<real_t>() {}
template <> ZBis<real_t>:: ~ZBis() noexcept {}

template <>
void ZBis<real_t>:: lookup(Triplet<real_t> &x, Triplet<real_t> &f, FunctionType &F)
{
    static const real_t half(0.5);

    real_t *x_neg = 0, *f_neg=0;
    real_t *x_pos = 0, *f_pos=0;

    switch( Sign::MakePairFrom(f.a,f.c) )
    {
        case ZP_Signs:
        case ZN_Signs:
            x.b = x.a;
            f.b = f.a = 0;
            return;

        case PZ_Signs:
        case NZ_Signs:
            x.b = x.c;
            f.b = f.c = 0;
            return;

        case NP_Signs:
            x_neg = &x.a; f_neg = &f.a;
            x_pos = &x.c; f_pos = &f.c;
            break;

        case PN_Signs:
            x_neg = &x.c; f_neg = &f.c;
            x_pos = &x.a; f_pos = &f.a;
            break;

        case ZZ_Signs:
            if( __Zero__ != Sign::Of( f.b = F( x.b = half*(x.a+x.c) ) ) )
                throw Specific::Exception(CallSign, "non zero-uniform function");
            return;

        case NN_Signs:
        case PP_Signs:
            throw Specific::Exception(CallSign, "invalid starting point");
    }

    assert(0!=x_neg);
    assert(0!=x_pos);
    assert(0!=f_neg);
    assert(0!=f_pos);

    real_t width = Fabs<real_t>::Of(x.c-x.a);

    while(true)
    {
        switch( Sign::Of( f.b = F( x.b = half*(x.a+x.c) ) ) )
        {
            case __Zero__: return; // early return
            case Negative: *f_neg = f.b; *x_neg= x.b; break;
            case Positive: *f_pos = f.b; *x_pos= x.b; break;
        }
        const real_t newWidth = Fabs<real_t>::Of(x.c-x.a);
        if(newWidth>=width)
            return;
        width = newWidth;
    }

}
