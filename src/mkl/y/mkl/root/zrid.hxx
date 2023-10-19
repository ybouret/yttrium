
template <> ZRid<real_t>::  ZRid() noexcept : ZFind<real_t>() {}
template <> ZRid<real_t>:: ~ZRid() noexcept {}


template <>
const char * ZRid<real_t>:: callSign() const noexcept { return CallSign; }


namespace
{
    static inline
    void update(Triplet<real_t> &x, Triplet<real_t> &f, const real_t x_r, const real_t f_r)
    {
        real_t xx[4] = { x.a, x.b, x.c, x_r };
        real_t ff[4] = { f.a, f.b, f.c, f_r };
        {
            real_t * const ax = Memory::OutOfReach::Self(xx)-1;
            real_t * const af = Memory::OutOfReach::Self(ff)-1;
            NetworkSort::Algo<4>::Increasing(ax,af);
        }


        for(size_t i=0;i<3;++i)
        {

            const real_t f_lo = ff[i], f_hi = ff[i+1];
            switch( Sign::MakePairFrom(f_lo,f_hi) )
            {
                case PP_Signs:
                case NN_Signs:
                    continue;

                case ZP_Signs:
                case PZ_Signs:
                case NZ_Signs:
                case ZN_Signs:
                case ZZ_Signs:
                    throw Specific::Exception(CallSign,"found illogic zero in update");

                case NP_Signs:
                case PN_Signs:
                    x.a = xx[i];
                    x.c = xx[i+1];
                    f.a = f_lo;
                    f.c = f_hi;
                    if( Fabs<real_t>::Of(f_lo) <= Fabs<real_t>::Of(f_hi) )
                    {
                        x.b = x.a;
                        f.b = f.a;
                    }
                    else
                    {
                        x.b = x.c;
                        f.b = f.c;
                    }
                    return;
            }

        }

        throw Specific::Exception(CallSign, "singular function");


    }
}

template <>
void ZRid<real_t>:: lookup(Triplet<real_t> &x, Triplet<real_t> &f, FunctionType &F)
{
    static const real_t zero(0);

    //--------------------------------------------------------------------------
    //
    // increasing order a->c
    //
    //--------------------------------------------------------------------------
    if(x.a>x.c)
    {
        Swap(x.a,x.c);
        Swap(f.a,f.c);
    }
    assert(x.a<=x.c);

    //--------------------------------------------------------------------------
    //
    // init
    //
    //--------------------------------------------------------------------------
    Handle xp = {0,0}, fp = {0,0};
    if( found(xp,fp,x,f,F)) return;

    real_t width = Fabs<real_t>::Of(x.c-x.a); // avoid roundoff
    while(true)
    {
        assert( Sign::Of(f.a) != __Zero__ );
        assert( Sign::Of(f.c) != __Zero__ );
        bool positive = true;
        switch( Sign::Of(f.b = F( x.b = half * (x.a+x.c) )) )
        {
            case __Zero__: f.b=zero; return;
            case Positive:
                break;
            case Negative:
                positive = false;
                break;
        }


        const real_t fbfb  = f.b * f.b;
        const real_t fafc  = f.a * f.c; assert(fafc<=zero);
        const real_t Delta = fbfb-fafc;
        if(Delta<=fbfb)
        {
            // rounding error
            goto BISECTION;
        }
        else
        {
            real_t fac = half * width * (f.b/Sqrt<real_t>::Of(Delta));
            if(f.c < f.a) fac = -fac;
            const real_t x_r  = Clamp(x.a,x.b-fac,x.c);
            const real_t f_r = F(x_r);
            if( __Zero__ == Sign::Of(f_r) )
            {
                x.b = x_r;
                f.b = zero;
                return;
            }
            else
            {
                update(x,f,x_r,f_r);
                goto END;
            }
        }

        assert( Say("never get here") );

    BISECTION:
        if(positive)
        {
            *xp.pos = x.b;
            *fp.pos = f.b;
        }
        else
        {
            *xp.neg = x.b;
            *fp.neg = f.b;
        }

    END:
        const real_t newWidth = Fabs<real_t>::Of(x.c-x.a);
        if(newWidth>=width)
            return;
        width = newWidth;
    }




}


