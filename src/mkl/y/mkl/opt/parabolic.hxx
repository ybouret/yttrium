


//! append fourth value
static inline void append(const real_t xm, real_t xx[], real_t ff[], Function<real_t,real_t> &F)
{
    ff[3] = F( xx[3] = xm );
}

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

namespace {

    //! upgrade
    static inline
    void upgrade(const real_t   xm,
                 real_t * const xx,
                 real_t * const ff,
                 size_t        &nn,
                 Function<real_t,real_t> &F)
    {
        const size_t i = nn++;
        ff[i] = F( xx[i] = xm );
    }

    static inline
    void upgradeMiddle(const Triplet<real_t> &x,
                       real_t * const xx,
                       real_t * const ff,
                       size_t        &nn,
                       Function<real_t,real_t> &F)
    {
        static const real_t half(0.5);
        upgrade(Clamp(x.a,half*(x.a+x.c),x.c),xx,ff,nn,F);
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
        if( fm < f.b )
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
        if( fm < f.b )
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

    const real_t width        = x.c-x.a;
    const real_t mu_a         = Max(f.a - f.b,zero);
    const real_t mu_c         = Max(f.c - f.b,zero);
    const real_t beta         = Clamp(zero,(x.b-x.a)/width,one);
    const real_t oneMinusBeta = one-beta;
    const real_t delta        = beta*(oneMinusBeta) * ((mu_a-mu_c)/(oneMinusBeta*mu_a+beta * mu_c) );
    const real_t uQuadratic   = Clamp(zero,half*(delta+one),one);
    const real_t xQuadratic   = Clamp(x.a, x.a+(width*uQuadratic), x.c);
    const real_t fQuadratic   = F(xQuadratic);

    if(fQuadratic<f.b)
    {
        //----------------------------------------------------------------------
        //
        // found a new matching point : keep it and the two smallest values
        //
        //----------------------------------------------------------------------
        std::cerr << "Found At " << xQuadratic << " -> " << fQuadratic << std::endl;

        if(mu_a<=mu_c)
        {

        }
        else
        {

        }


        return true;
    }
    else
    {
        //----------------------------------------------------------------------
        //
        // f.b is still the one
        //
        //----------------------------------------------------------------------

        return false;
    }



#if 0
    real_t xx[6] = { x.a,x.b,x.c,zero,zero,zero };
    real_t ff[6] = { f.a,f.b,f.c,zero,zero,zero };
    size_t nn    = 3;


    if(x.b<=x.a||x.c<=x.b)
    {
        //----------------------------------------------------------------------
        //
        // value on one side: probe middle
        //
        //----------------------------------------------------------------------
        upgradeMiddle(x, xx, ff, nn, F);
    }
    else
    {
        const real_t width = x.c-x.a;
        const real_t mu_a  = Fabs<real_t>::Of(f.a - f.b);
        const real_t mu_c  = Fabs<real_t>::Of(f.c - f.b);

        if(mu_a<=zero && mu_c <= zero)
        {
            upgradeMiddle(x, xx, ff, nn, F);
        }
        else
        {
            assert(mu_a>zero || mu_c>zero);
            const real_t beta         = Clamp(zero,(x.b-x.a)/width,one);
            const real_t oneMinusBeta = one-beta;
            const real_t delta        = beta*(oneMinusBeta) * ((mu_a-mu_c)/(oneMinusBeta*mu_a+beta * mu_c) );
            const real_t uOpt         = Clamp(zero,half*(delta+one),one);
            const real_t wOpt         = width * uOpt;
            upgrade(Clamp(x.a, x.a+wOpt, x.c),xx,ff,nn,F);
        }
        HeapSort::Tableau(xx,nn,Comparison::Increasing<real_t>,ff);
        real_t wmax = xx[1]-xx[0];
        size_t jmax = 1;
        for(size_t j=2;j<nn;++j)
        {
            const real_t wtmp = xx[j] - xx[j-1];
            if(wtmp>wmax)
            {
                jmax = j;
                wmax = wtmp;
            }
        }
        const real_t xlo = xx[jmax-1];
        const real_t xhi = xx[jmax];
        upgrade(Clamp(xlo,half*(xlo+xhi),xhi),xx,ff,nn,F);

    }

    HeapSort::Tableau(xx,nn,Comparison::Increasing<real_t>,ff);
    chooseTriplets(x,f, xx, ff, nn);
#endif

}
