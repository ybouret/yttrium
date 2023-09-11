

template <>
RK45<real_t>:: RK45(const StepType &s) noexcept :
Actuator<real_t>(s),
code(0)
{
}


template <>
RK45<real_t>:: RK45(const StepType &s,
                    const size_t   n):
Actuator<real_t>(s),
code( n <=0 ? 0 : new Code(n) )
{
}


template <>
RK45<real_t>:: ~RK45() noexcept
{
    Nullify(code);
}

template <>
void RK45<real_t>::move(Writable<real_t>  &       y,
                        const Readable<real_t>  & dydx,
                        real_t &                  x,
                        real_t                    htry,
                        const real_t              eps,
                        const Readable<real_t> &  yscal,
                        real_t  &                 hdid,
                        real_t  &                 hnext,
                        Equation &                drvs,
                        Callback *                cntl)
{
    assert(y.size() == dydx.size());
    assert(y.size() == yscal.size());
    
    const size_t n= y.size();
    if(0==code||code->dims<n)
    {
        Nullify(code);
        code = new Code(n);
    }
    assert(0!=code);
    code->move(*step, y, dydx, x, htry, eps, yscal, hdid, hnext, drvs, cntl);
}
