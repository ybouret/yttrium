

template <>
RK45<real_t>:: RK45() noexcept : code(0)
{
}


template <>
RK45<real_t>:: RK45(const size_t n): code( n <=0 ? 0 : new Code(n) )
{
}


template <>
RK45<real_t>:: ~RK45() noexcept
{
    Nullify(code);
}

template <>
void RK45<real_t>::move(RK45_Step<real_t> &       step,
                        Writable<real_t>  &       y,
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
    const size_t n= y.size();
    if(0==code||code->dims<n)
    {
        Nullify(code);
        code = new Code(n);
    }
    assert(0!=code);
    code->move(step, y, dydx, x, htry, eps, yscal, hdid, hnext, drvs, cntl);
}
