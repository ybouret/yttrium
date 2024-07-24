
namespace
{
    static inline void CoSort(real_t * const xx,
                              real_t * const ff,
                              const size_t   nn)
    {
        HeapSort::Tableau(xx, nn, Comparison::Increasing<real_t>, ff);
    }

    //------------------------------------------------------------------------------
    //
    //
    //  choose best triplets among selected points
    //
    //
    //------------------------------------------------------------------------------
    static inline
    void ChooseNewTriplet(Triplet<real_t> & x,
                          Triplet<real_t> & f,
                          real_t  * const   xx,
                          real_t  * const   ff,
                          const size_t      nn)
    {
        assert(nn>=3);
        CoSort(xx,ff,nn);
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

    static inline
    bool HasConverged(const Triplet<real_t> &x)
    {
        static const real_t _3(3);
        static const real_t tol = _3 *  Numeric<real_t>::SQRT_EPSILON;
        return WidthOf(x) <= SumOfAbs(x) * tol;
    }
}


template <>
bool Minimize<real_t>:: Found(Triplet<real_t> &x,
                              Triplet<real_t> &f,
                              FunctionType    &F)
{
    static const real_t zero(0);
    static const real_t one(1);
    static const real_t half(0.5);
    static const real_t R    = Numeric<real_t>::INV_GOLDEN;
    static const real_t C    = one-R;
    static const real_t utol = Numeric<real_t>::SQRT_EPSILON;

    assert(x.isIncreasing());
    assert(f.isLocalMinimum());
    
    Y_MINIMIZE_PRINT("@init : " << x << " -> " << f);
    {
        const real_t width = x.c-x.a;
        real_t       xx[4] = { x.a, x.b, x.c, zero };
        real_t       ff[4] = { f.a, f.b, f.c, zero };
        size_t       nn    = 3;
        //----------------------------------------------------------------------
        //
        //
        // Special case: x.b at left
        //
        //
        //----------------------------------------------------------------------
        if(x.b <= x.a)
        {
            x.a    = x.b;
            f.a    = f.b;
            ff[nn] = F( xx[nn] = Clamp(x.a,x.a+C*width,x.c) );
            ++nn;
            ChooseNewTriplet(x,f,xx,ff,nn);
            Y_MINIMIZE_PRINT("@left : " << x << " -> " << f);
            return HasConverged(x);
        }

        //----------------------------------------------------------------------
        //
        //
        // Special case: x.b at right
        //
        //
        //----------------------------------------------------------------------
        if(x.c<=x.b)
        {
            x.c    = x.b;
            f.c    = f.b;
            ff[nn] = F( xx[nn] = Clamp(x.a,x.c-C*width,x.c) );
            ++nn;
            ChooseNewTriplet(x,f,xx,ff,nn);
            Y_MINIMIZE_PRINT("@right: " << x << " -> " << f);
            return HasConverged(x);
        }

        //----------------------------------------------------------------------
        //
        //
        // Generic case in : regularize
        //
        //
        //----------------------------------------------------------------------
        assert(x.a<x.b);
        assert(x.b<x.c);

        {
            const real_t l = Max(x.b-x.a,zero);
            const real_t r = Max(x.c-x.b,zero);
            if( l >= r)
            {
                // left >= right
                xx[nn] = Clamp(x.a,x.b-C*l,x.b);
            }
            else
            {
                // right > left
                xx[nn] = Clamp(x.b,x.b+C*r,x.c);
            }
            ff[nn] = F( xx[nn] );
            ++nn;
        }
        ChooseNewTriplet(x,f,xx,ff,nn);
        Y_MINIMIZE_PRINT("@loop : " << x << " -> " << f);

        if(false)
        {
            OutputFile fp("min.dat");
            for(size_t i=0;i<nn;++i)
            {
                fp("%g %g 3\n", double(xx[i]), double(ff[i]) );
            }
            fp << "\n";
        }
        assert(x.isIncreasing());
        assert(f.isLocalMinimum());
    }

    //--------------------------------------------------------------------------
    //
    //
    // sort out result
    //
    //
    //--------------------------------------------------------------------------
    if( HasConverged(x) ) return true;
    if(x.b<=x.a)          return false;
    if(x.c<=x.b)          return false;

    //--------------------------------------------------------------------------
    //
    //
    // try parabolic interpolation
    //
    //
    //--------------------------------------------------------------------------
    assert(x.a<x.b);
    assert(x.b<x.c);

    {
        //----------------------------------------------------------------------
        //
        // compute new width and metrics elements
        //
        //----------------------------------------------------------------------
        const real_t          width = x.c - x.a; assert(width>zero);
        const real_t          mu_a  = Max(f.a - f.b,zero);
        const real_t          mu_c  = Max(f.c - f.b,zero);
        const real_t          beta  = Clamp(zero,(x.b-x.a)/width,one);
        const Triplet<real_t> u     = {zero,beta,one};
        real_t                u_opt = half;

        //----------------------------------------------------------------------
        //
        // compute reduced parabolic position
        //
        //----------------------------------------------------------------------
        if(mu_a<=zero)
        {
            if(mu_c<=zero)
            {
                assert(mu_a<=zero); assert(mu_c<=zero);
                u_opt = half;
            }
            else
            {
                assert(mu_a<=zero); assert(mu_c>zero);
                u_opt = Clamp(zero,half*beta,one);
            }

        }
        else
        {
            assert(mu_a>zero);
            if(mu_c<=zero)
            {
                assert(mu_a>zero); assert(mu_c<=zero);
                u_opt = Clamp(zero,half*(one+beta),one);
            }
            else
            {
                assert(mu_a>zero); assert(mu_c>zero);
                const real_t oneMinusBeta = one-beta;
                const real_t delta        = beta*(oneMinusBeta) * ((mu_a-mu_c)/(oneMinusBeta*mu_a+beta * mu_c) );
                u_opt                     = Clamp(zero,half*(delta+one),one);
            }
        }

        //----------------------------------------------------------------------
        //
        // check meaningfull parabolic step
        //
        //----------------------------------------------------------------------
        for(size_t i=3;i>0;--i)
        {
            if(Fabs<real_t>::Of(u[i]-u_opt)<=utol) 
            {
                Y_MINIMIZE_PRINT("no interpolation...");
                return false;
            }
        }

        const real_t xp    = Clamp(x.a, x.a + width * u_opt, x.c);
        real_t       xx[4] = { x.a, x.b, x.c, xp    };
        real_t       ff[4] = { f.a, f.b, f.c, F(xp) };
        ChooseNewTriplet(x, f, xx, ff, 4);
        Y_MINIMIZE_PRINT("@inter: " << x << " -> " << f);

        if(false)
        {
            OutputFile fp("min.dat",true);
            for(size_t i=0;i<4;++i)
            {
                fp("%g %g 4\n", double(xx[i]), double(ff[i]));
            }
            fp << "\n";
        }

        return HasConverged(x);
    }


}



template <>
real_t Minimize<real_t>:: Locate(const Minimizing::Prolog prolog, 
                                 Triplet<real_t>         &x,
                                 Triplet<real_t>         &f,
                                 FunctionType            &F)
{

    //assert(x.isIncreasing());
    // assert(f.isLocalMinimum());

    switch (prolog) 
    {
        case Minimizing::Direct:
            if(!x.isIncreasing())   throw Specific::Exception(CallSign,"abscissae are not increasing");
            if(!f.isLocalMinimum()) throw Specific::Exception(CallSign,"ordinates are not local minimum");
            break;

        case Minimizing::Inside: {
            const Temporary<bool> flag(Bracketing::Verbose,Minimizing::Verbose);
            if(!Bracket<real_t>::Inside(x,f,F)) return x.b; // early return
        } break;

        case Minimizing::Expand: {
            const Temporary<bool> flag(Bracketing::Verbose,Minimizing::Verbose);
            Bracket<real_t>::Expand(x,f,F);
        } break;
    }

    Y_MINIMIZE_PRINT("<Locate>");
    size_t cycle = 0;
    while( !Found(x,f,F) )
    {
        ++cycle;
        Y_MINIMIZE_PRINT("@cycle = " <<cycle);
    }
    Y_MINIMIZE_PRINT("@find : F(" << x.b << ") = " << f.b);
    Y_MINIMIZE_PRINT("<Locate/>");

    return x.b;

}
