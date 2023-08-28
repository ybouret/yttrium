

static inline real_t getBeta(const Triplet<real_t> &x)
{
    assert(x.isIncreasing());
    if(x.b<=x.a)
        return static_cast<real_t>(0);
    else
    {
        if(x.c<=x.b)
        {
            return static_cast<real_t>(1);
        }
        else
        {
            assert(x.a<x.c);
            return (x.b-x.a)/(x.c-x.a);
        }
    }
}

template <>
void Parabolic<real_t>:: Step(Triplet<real_t> &x, Triplet<real_t> &f, FunctionType &F)
{
    assert(x.isIncreasing());
    assert(f.isLocalMinimum());

    const real_t beta = getBeta(x);
    

}
