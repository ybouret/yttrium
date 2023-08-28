


static inline void createTriplets(Triplet<real_t> &      x,
                                  Triplet<real_t> &      f,
                                  const real_t *   const xx,
                                  const real_t *   const ff,
                                  const unsigned * const id)
{
    for(size_t j=3;j>0;--j)
    {
        const unsigned J = id[j];
        x[j] = xx[J];
        f[j] = ff[J];
    }
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
    createTriplets(x,f,xx,ff,indx[0]);
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
            const real_t beta = (x.b-x.a)/(x.c-x.a);
        }
    }



}
