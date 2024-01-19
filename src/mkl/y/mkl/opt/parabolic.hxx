


//------------------------------------------------------------------------------
//
//
//  choose best triplets among selected points
//
//
//------------------------------------------------------------------------------
static inline
void chooseTriplets(Triplet<real_t> & x,
                    Triplet<real_t> & f,
                    const real_t      xx[],
                    const real_t      ff[],
                    const size_t      nn)
{
    assert(nn>=3);
    const size_t nt = nn-2; // number of triplets
    size_t       it = 0;    // index of triplet in [0:nt-1]

    //--------------------------------------------------------------------------
    //
    // MUST find a first local minimum
    //
    //--------------------------------------------------------------------------
INITIALIZE:
    x.load(xx+it);
    f.load(ff+it);
    if(!f.isLocalMinimum())
    {
        if(++it>=nt) return throw Specific::Exception(CallSign,"no local minimum!");
        goto INITIALIZE;
    }

    //--------------------------------------------------------------------------
    //
    // Loop over following triplets
    //
    //------------------------------------------------------------------------------
    assert(f.isLocalMinimum());
    real_t w = Fabs<real_t>::Of(x.c-x.a);
    for(++it;it<nt;++it)
    {
        const size_t    i1 = it+1;
        const size_t    i2 = it+2;
        Triplet<real_t> tx = { xx[it], xx[i1], xx[i2] };
        Triplet<real_t> tf = { ff[it], ff[i1], ff[i2] };

        if( tf.isLocalMinimum() )
        {
            const real_t tw = Fabs<real_t>::Of(tx.c-tx.a);
            switch( Sign::Of(tf.b,f.b) )
            {
                case Positive: assert(tf.b>f.b); continue;
                case Negative: assert(tf.b<f.b); goto IMPROVE;
                case __Zero__:
                    if(tw<w) goto IMPROVE;
                    continue;

            }

        IMPROVE:
            Memory::OutOfReach::Copy(x,tx);
            Memory::OutOfReach::Copy(f,tf);
            w=tw;
        }
    }
}

 namespace
{
    static inline
    void tryAppend(const real_t           u,
                   const Triplet<real_t> &x,
                   const real_t           w,
                   real_t * const         uu,
                   real_t * const         xx,
                   real_t * const         ff,
                   size_t  &              nn,
                   Function<real_t,real_t> &F)
    {
        static const real_t utol = Numeric<real_t>::SQRT_EPSILON;
        assert(u>=static_cast<real_t>(0));
        assert(u<=static_cast<real_t>(1));

        for(size_t i=0;i<nn;++i)
        {
            if( Fabs<real_t>::Of(u-uu[i]) <= utol ) return;
        }
        uu[nn] = u;
        ff[nn] = F( xx[nn] = Clamp(x.a,x.a+u*w,x.c) );
        ++nn;
    }
}

template <>
bool Parabolic<real_t>:: Step(Triplet<real_t> &x, Triplet<real_t> &f, FunctionType &F)
{

    static const real_t half(0.5);
    static const real_t zero(0);
    static const real_t one(1);

    assert(x.isIncreasing());
    assert(f.isLocalMinimum());


    
    //--------------------------------------------------------------------------
    //
    // At Left
    //
    //--------------------------------------------------------------------------
    if(x.b <= x.a)
    {
        const real_t xm = Clamp(x.a,half*(x.a+x.c),x.c);
        const real_t fm = F(xm);
        f.a = f.b;
        x.a = x.b;
        if( fm <= f.b )
        {
            // explore middle
            x.b = xm;
            f.b = fm;
            return true;
        }
        else
        {
            // tighten right side
            x.c = xm;
            f.c = fm;
            return false;
        }
    }



    //--------------------------------------------------------------------------
    //
    // At Right
    //
    //--------------------------------------------------------------------------
    if(x.c <= x.b )
    {
        const real_t xm = Clamp(x.a,half*(x.a+x.c),x.c);
        const real_t fm = F(xm);
        f.c = f.b;
        x.c = x.b;
        if( fm <= f.b )
        {
            // explore middle
            x.b = xm;
            f.b = fm;
            return true;
        }
        else
        {
            // tighten left side
            x.a = xm;
            f.a = fm;
            return false;
        }
    }

    //--------------------------------------------------------------------------
    //
    // In Bulk
    //
    //--------------------------------------------------------------------------
    assert(x.a<x.b); assert(x.b<x.c);


    const real_t mu_a         = Max(f.a - f.b,zero);
    const real_t mu_c         = Max(f.c - f.b,zero);
    const real_t width        = x.c-x.a;
    const real_t beta         = Clamp(zero,(x.b-x.a)/width,one);

    real_t       uu[5]        = { zero, beta, one, zero, zero };
    real_t       xx[5]        = { x.a,  x.b,  x.c, zero, zero };
    real_t       ff[5]        = { f.a,  f.b,  f.c, zero, zero };
    size_t       nn           = 3;

    if(mu_a<=zero)
    {
        if(mu_c<=zero)
        {
            assert(mu_a<=zero); assert(mu_c<=zero);
            const real_t u = half;
            tryAppend(u,x,width,uu,xx,ff,nn,F);
        }
        else
        {
            assert(mu_a<=zero); assert(mu_c>zero);
            const real_t u = Clamp(zero,half*beta,one);
            tryAppend(u,x,width,uu,xx,ff,nn,F);
        }

    }
    else
    {
        assert(mu_a>zero);
        if(mu_c<=zero)
        {
            assert(mu_a>zero); assert(mu_c<=zero);
            const real_t u = Clamp(zero,half*(one+beta),one);
            tryAppend(u,x,width,uu,xx,ff,nn,F);
        }
        else
        {
            assert(mu_a>zero); assert(mu_c>zero);
            const real_t oneMinusBeta = one-beta;
            const real_t delta        = beta*(oneMinusBeta) * ((mu_a-mu_c)/(oneMinusBeta*mu_a+beta * mu_c) );
            const real_t u            = Clamp(zero,half*(delta+one),one);
            tryAppend(u,x,width,uu,xx,ff,nn,F);
        }

    }

    return false;



}
