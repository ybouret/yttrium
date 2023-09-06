
template <>
Derivatives<real_t>:: ~Derivatives() noexcept
{
    assert(0!=code);
    delete code;
    code = 0;
}


template <>
Derivatives<real_t>:: Derivatives()  : Kernel::Derivatives(),
code( new Code() )
{

}

template <>
real_t Derivatives<real_t>:: eval_(FunctionType &F, const real_t xlo, const real_t x, const real_t xhi)
{
    assert(0!=code);
    const Triplet<real_t> xx = {xlo, x, xhi };
    const real_t          Fb = F(xx.b);
    return code->eval(F,xx,Fb);
}
 
template <>
void Derivatives<real_t>:: setMetrics( Triplet<real_t> &x, const real_t x0,   real_t &length, const Interval<real_t> &I) const
{
    assert(0!=code);
    return Code::SetMetrics(x,x0,length,I);
}

template <>
real_t Derivatives<real_t>:: compute_(FunctionType &F, const real_t x0, const real_t h, const Interval<real_t> &I)
{
    assert(0!=code);
    return code->compute(F,x0, h,I, 0);
}
