
namespace
{

    //------------------------------------------------------------------------------
    //
    //
    //  choose best triplets among selected points
    //
    //
    //------------------------------------------------------------------------------
    static inline
    void chooseNewTriplet(Triplet<real_t> & x,
                          Triplet<real_t> & f,
                          real_t  * const   xx,
                          real_t  * const   ff,
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

    static inline void coSort(real_t * const xx,
                              real_t * const ff,
                              const size_t   nn)
    {
        HeapSort::Tableau(xx, nn, Comparison::Increasing<real_t>, ff);
    }

    static inline void coSort4(real_t * const xx,
                               real_t * const ff)
    {
        real_t * const abscissae = Memory::OutOfReach::Cast<real_t>(xx)-1;
        real_t * const ordinates = Memory::OutOfReach::Cast<real_t>(ff)-1;
        NetworkSort::Algo<4>::Increasing(abscissae,ordinates);
    }
}

#if 0
namespace
{
    static inline
    void appendTrial(const real_t           u,
                     const Triplet<real_t> &x,
                     const real_t           w,
                     real_t * const         xx,
                     real_t * const         ff,
                     size_t  &              nn,
                     Function<real_t,real_t> &F)
    {
        assert(u>=static_cast<real_t>(0));
        assert(u<=static_cast<real_t>(1));


        ff[nn] = F( xx[nn] = Clamp(x.a,x.a+u*w,x.c) );
        std::cerr << "appendTrial(" << xx[nn] << ") to " << x << std::endl;
        ++nn;
    }
}
#endif

template <>
void Minimize<real_t>:: Step(Triplet<real_t> &x,
                             Triplet<real_t> &f,
                             FunctionType    &F)
{
    static const real_t zero(0);
    static const real_t one(1);
    static const real_t half(0.5);
    static const real_t R   = Numeric<real_t>::INV_GOLDEN;
    static const real_t C   = one-R;

    assert(x.isIncreasing());
    assert(f.isLocalMinimum());

    std::cerr << "C=" << C << std::endl;

    real_t xx[4] = { x.a, x.b, x.c, zero };
    real_t ff[4] = { f.a, f.b, f.c, zero };
    size_t nn    = 3;
    real_t width = x.c-x.a;

      //--------------------------------------------------------------------------
    //
    // x.b at left
    //
    //--------------------------------------------------------------------------
    if(x.b <= x.a)
    {
        x.a    = x.b;
        f.a    = f.b;
        ff[nn] = F( xx[nn] = Clamp(x.a,x.a+C*width,x.c) );
        ++nn; assert(4==nn);
        coSort4(xx,ff);
        chooseNewTriplet(x,f,xx,ff,nn);
        return;
    }

    //--------------------------------------------------------------------------
    //
    // x.b at right
    //
    //--------------------------------------------------------------------------
    if(x.c<=x.b)
    {
        x.c    = x.b;
        f.c    = f.b;
        ff[nn] = F( xx[nn] = Clamp(x.a,x.c-C*width,x.c) );
        ++nn; assert(4==nn);
        coSort4(xx,ff);
        chooseNewTriplet(x,f,xx,ff,nn);
        return;
    }

    // in bulk
    assert(x.a<x.b);
    assert(x.b<x.c);

    const real_t l = Max(x.b-x.a,zero);
    const real_t r = Max(x.c-x.b,zero);
    if( r < l )
    {
        // take left
        xx[nn] = Clamp(x.a,x.b-C*l,x.b);
    }
    else
    {
        // take right
        xx[nn] = Clamp(x.b,x.b+C*r,x.c);
    }

    ff[nn] = F( xx[nn] );
    ++nn; assert(4==nn);
    coSort4(xx,ff);
    Core::Display(std::cerr << "xx=", xx, nn) << " -> ";
    Core::Display(std::cerr << "ff=", ff, nn) << std::endl;

    {
        Libc::OutputFile fp("min.dat");
        for(size_t i=0;i<nn;++i)
        {
            fp("%g %g\n", double(xx[i]), double(ff[i]) );
        }
    }

    chooseNewTriplet(x,f,xx,ff,nn);

    assert(x.isIncreasing());
    assert(f.isLocalMinimum());
    

    if(x.b<=x.a) return;
    if(x.c<=x.b) return;
    assert(x.a<x.b);
    assert(x.b<x.c);
    width             = x.c - x.a; assert(width>zero);
    const real_t mu_a = Max(f.a - f.b,zero);
    const real_t mu_c = Max(f.c - f.b,zero);
    const real_t beta = Clamp(zero,(x.b-x.a)/width,one);




    throw Exception("todo");


#if 0
    const real_t mu_a         = Max(f.a - f.b,zero);
    const real_t mu_c         = Max(f.c - f.b,zero);
    const real_t beta         = Clamp(zero,(x.b-x.a)/width,one);

    if(mu_a<=zero)
    {
        if(mu_c<=zero)
        {
            assert(mu_a<=zero); assert(mu_c<=zero);
            const real_t u = half;
            appendTrial(u,x,width,xx,ff,nn,F);
        }
        else
        {
            assert(mu_a<=zero); assert(mu_c>zero);
            const real_t u = Clamp(zero,half*beta,one);
            appendTrial(u,x,width,xx,ff,nn,F);
        }

    }
    else
    {
        assert(mu_a>zero);
        if(mu_c<=zero)
        {
            assert(mu_a>zero); assert(mu_c<=zero);
            const real_t u = Clamp(zero,half*(one+beta),one);
            appendTrial(u,x,width,xx,ff,nn,F);
        }
        else
        {
            assert(mu_a>zero); assert(mu_c>zero);
            const real_t oneMinusBeta = one-beta;
            const real_t delta        = beta*(oneMinusBeta) * ((mu_a-mu_c)/(oneMinusBeta*mu_a+beta * mu_c) );
            const real_t u            = Clamp(zero,half*(delta+one),one);
            appendTrial(u,x,width,xx,ff,nn,F);
        }
    }

    assert(4==nn);
    const bool decreased = ff[nn-1] <= f.b;
    HeapSort::Tableau(xx, nn, Comparison::Increasing<real_t>, ff);

    std::cerr << " *** decreased = " << decreased << std::endl;

    if(!decreased)
    {
        std::cerr << "Error..." << std::endl;
        throw Exception("not decreased");
    }

    chooseNewTriplet(x, f, xx, ff, nn);
#endif
}

namespace
{
    static inline real_t WidthOf(const Triplet<real_t> &x)
    {
        return Fabs<real_t>::Of(x.c-x.a);
    }

    static inline real_t SumOfAbs(const Triplet<real_t> &x)
    {
        const real_t aa = Fabs<real_t>::Of(x.a);
        const real_t ab = Fabs<real_t>::Of(x.b);
        const real_t ac = Fabs<real_t>::Of(x.c);
        return Antelope::Sum3<real_t>::Of(aa,ab,ac);
    }
}

template <>
real_t Minimize<real_t>:: Find(Triplet<real_t> &x, Triplet<real_t> &f, FunctionType &F)
{
    static const real_t _3(3);
    static const real_t tol = _3 *  Numeric<real_t>::SQRT_EPSILON;

    assert(x.isIncreasing());
    assert(f.isLocalMinimum());

    std::cerr << "@init: " << x << " -> " << f << std::endl;
    while( WidthOf(x) > SumOfAbs(x) * tol  )
    {
        Step(x,f,F);
        std::cerr << "@loop: " << x << " -> " << f << std::endl;
    }

    return x.b;

}
