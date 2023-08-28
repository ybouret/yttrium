

template <>
void Parabolic<real_t>:: Step(Triplet<real_t> &x, Triplet<real_t> &f, FunctionType &F)
{
    assert(x.a<=x.b); assert(x.b<=x.c);
    assert(f.isLocalMinimum());
    

}
