
template <>
Integrator<real_t>:: Integrator(const real_t userEps) noexcept :
eps(userEps),
code(0)
{
    
}

template <>
Integrator<real_t>:: Integrator(const real_t userEps, const size_t n) :
eps(userEps),
code( n<= 0 ? 0 : new Code(n) )
{

}

template <>
Integrator<real_t>:: ~Integrator() noexcept
{
    Nullify(code);
}

template <>
void Integrator<real_t>:: run(Writable<real_t> & ystart,
                              const real_t       x1,
                              const real_t       x2,
                              real_t &           hini,
                              Equation &         drvs,
                              Callback *         cntl,
                              Actuator<real_t> & actuator)
{
    const size_t n = ystart.size();
    assert(n>0);
    if(0==code||code->dims<n)
    {
        Nullify(code);
        code = new Code(n);
    }
    return code->run(ystart, x1, x2, hini, drvs, cntl, actuator, eps);
}
