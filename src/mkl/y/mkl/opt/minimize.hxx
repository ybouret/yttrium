

template <>
real_t Minimize<real_t>:: Locate(Triplet<real_t> &x,
                                 Triplet<real_t> &f,
                                 FunctionType    &F)
{
    static const real_t zero(0);

    assert(x.isIncreasing());
    assert(f.isLocalMinimum());

    real_t width = 0;
    bool   state = Parabolic<real_t>::Step(x,f,F,width);

    if(state && width<=zero)
    {
        return 0;
    }

    


    return zero;

}

