

template <>
real_t Minimize<real_t>:: Locate(Triplet<real_t> &x,
                                 Triplet<real_t> &f,
                                 FunctionType    &F)
{
    static const real_t zero(0);

    assert(x.isIncreasing());
    assert(f.isLocalMinimum());

    return zero;

}

