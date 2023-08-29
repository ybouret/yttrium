


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

template <>
void Parabolic<real_t>:: Step(Triplet<real_t> &x, Triplet<real_t> &f, FunctionType &F)
{
    static const real_t half(0.5);
    static const real_t zero(0);
    static const real_t one(1);

    assert(x.isIncreasing());
    assert(f.isLocalMinimum());

    real_t xx[4] = { x.a,x.b,x.c,zero };
    real_t ff[4] = { f.a,f.b,f.c,zero };
    
    if(x.b<=x.a||x.c<=x.b)
    {
        //----------------------------------------------------------------------
        //
        // value on one side: probe middle
        //
        //----------------------------------------------------------------------
        append(Clamp(x.a,half*(x.a+x.c),x.c),xx,ff,F);
    }
    else
    {
        const real_t width =  x.c-x.a;
        const real_t beta  = (x.b-x.a)/width;
        const real_t mu_a  = Fabs<real_t>::Of(f.a - f.b);
        const real_t mu_c  = Fabs<real_t>::Of(f.c - f.b);
        real_t       uopt  = half;
        bool         used  = false;
        if(mu_a<=zero)
        {
            //------------------------------------------------------------------
            // mu_a = 0
            //------------------------------------------------------------------
            if(mu_c<=zero)
            {
                used = true;
                // mu_a=0, mu_c=0
                if(beta<half)
                {
                    // probe between b and c
                    append(Clamp(x.b,half*(x.b+x.c),x.c),xx,ff,F);
                }
                else
                {
                    // probe between a and b
                    append(Clamp(x.a,half*(x.a+x.b),x.b),xx,ff,F);
                }
            }
            else
            {
                // mu_a=0, mu_c>0
                uopt = half - (one-beta)*half;
            }
        }
        else
        {
            //------------------------------------------------------------------
            // mu_a > 0
            //------------------------------------------------------------------
            if(mu_c<=zero)
            {
                // mu_a>0, mu_c=0
                uopt = half + half * beta;
            }
            else
            {
                // mu_a>0, mu_c>0
                const real_t omb  = one - beta;
                uopt = half + half * beta * omb * (mu_a-mu_c)/(omb*mu_a+beta*mu_c);
            }
        }

        //----------------------------------------------------------------------
        // append value if not used
        //----------------------------------------------------------------------
        if(!used)
            append(Clamp(x.a,x.a+uopt*width,x.c),xx,ff,F);

        //----------------------------------------------------------------------
        // create local evaluation
        //----------------------------------------------------------------------
        {
            real_t * const _x = xx-1;
            real_t * const _f = ff-1;
            NetworkSort::Algo<4,real_t>::Increasing(_x, _f);
        }

        //----------------------------------------------------------------------
        // and choose the final triplets
        //----------------------------------------------------------------------
        chooseTriplets(x,f,xx,ff,4);

    }

}
