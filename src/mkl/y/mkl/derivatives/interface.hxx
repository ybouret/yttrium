
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
real_t Derivatives<real_t>:: compute1D(FunctionType &F, const real_t x0, const real_t h, const Interval<real_t> &I)
{
    assert(0!=code);
    return code->compute(F,x0, h,I);
}
