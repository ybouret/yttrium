


static inline void createTriplets(Triplet<real_t> &      x,
                                  Triplet<real_t> &      f,
                                  const real_t *   const xx,
                                  const real_t *   const ff,
                                  const unsigned * const id)
{
    for(size_t j=3;j>0;--j)
    {
        const unsigned J = id[j]; assert(J<4);
        x[j] = xx[J];
        f[j] = ff[J];
    }
    //std::cerr << "built : " << x << " -> " << f << std::endl;
}

static inline void updateTriplets(Triplet<real_t>      &x,
                                  Triplet<real_t>      &f,
                                  const real_t * const xx,
                                  const real_t * const ff)
{
    static const unsigned indx[4][4] =
    {
        {0xff,0,1,2},
        {0xff,0,1,3},
        {0xff,0,2,3},
        {0xff,1,2,3}
    };

    // find first local minimum
    unsigned i = 0;
INITIALIZE:
    createTriplets(x,f,xx,ff,indx[i]);
    if(!f.isLocalMinimum())
    {
        ++i;
        if(i>=4)
        {
            throw Specific::Exception(CallSign,"corrupted data!");
        }
        goto INITIALIZE;
    }

    // find better local minimum
    for(++i;i<4;++i)
    {
        Triplet<real_t> tx,tf;
        createTriplets(tx,tf,xx,ff,indx[i]);
        if(!tf.isLocalMinimum()) continue;
        if(tf.b<f.b)
        {
            // new winner
            Memory::OutOfReach::Copy(x,tx);
            Memory::OutOfReach::Copy(f,tf);
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

#if 0
    if(x.b<=x.a||x.c<=x.b)
    {
        const real_t x_m = Clamp(x.a,half*(x.a+x.c),x.c);
        const real_t f_m = F(x_m);
    }
#endif

    if(x.b<=x.a)
    {
        const real_t x_m   = Clamp(x.a,half*(x.a+x.c),x.c);
        const real_t f_m   = F(x_m);
        const real_t xx[4] = { x.a, x.b, x_m, x.c };
        const real_t ff[4] = { f.a, f.b, f_m, f.c };
        updateTriplets(x,f,xx,ff);
    }
    else
    {
        if(x.c <= x.b)
        {
            const real_t x_m   = Clamp(x.a,half*(x.a+x.c),x.c);
            const real_t f_m   = F(x_m);
            const real_t xx[4] = { x.a, x_m, x.b, x.c };
            const real_t ff[4] = { x.a, f_m, x.b, x.c };
            updateTriplets(x,f,xx,ff);
        }
        else
        {
            assert(x.a<x.c);
            const real_t width = x.c-x.a;
            const real_t beta = (x.b-x.a)/width;
            const real_t mu_a = Fabs<real_t>::Of(f.a - f.b);
            const real_t mu_c = Fabs<real_t>::Of(f.c - f.b);
            real_t       uopt = half;
            if(mu_a<=zero)
            {
                if(mu_c<=zero)
                {
                    uopt = half;
                }
                else
                {
                    uopt = half - (one-beta)*half;
                }
            }
            else
            {
                //mu_a>0
                if(mu_c<=zero)
                {
                    uopt = half + half * beta;
                }
                else
                {
                    const real_t omb = one - beta;
                    uopt = half + half * beta * omb * (mu_a-mu_c)/(omb*mu_a+beta*mu_c);
                }
            }
            //std::cerr << "uopt=" << uopt << std::endl;
            uopt = Clamp(zero,uopt,one);
            const real_t x_opt = Clamp(x.a,x.a+width*uopt,x.c);
            const real_t f_opt = F(x_opt);
            //std::cerr << "x_opt=" << x_opt << std::endl;
            //std::cerr << "f_opt=" << f_opt << std::endl;

            real_t xx[5] = {0,x.a,x.b,x.c,x_opt};
            real_t ff[5] = {0,f.a,f.b,f.c,f_opt};
            NetworkSort::Algo<4,real_t>::Increasing(xx,ff);
            //Core::Display(std::cerr,xx+1,4) << std::endl;
            //Core::Display(std::cerr,ff+1,4) << std::endl;
            updateTriplets(x,f,xx+1,ff+1);
        }
    }



}
