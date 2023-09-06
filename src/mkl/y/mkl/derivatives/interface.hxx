
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
real_t Derivatives<real_t>:: eval_(FunctionType &F, const real_t x, const real_t delta, const real_t length)
{
    assert(0!=code);
    const real_t Fx = F(x);
    return code->eval(F, x, Fx, delta, length);
}
