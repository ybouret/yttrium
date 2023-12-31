
template <>
CashKarp<real_t>:: ~CashKarp() noexcept
{
    delete code;
    code = 0;
}


template <>
CashKarp<real_t>:: CashKarp() noexcept : code(0)
{

}

template <>
CashKarp<real_t>:: CashKarp(const size_t n) : code(  n <= 0 ? 0 : new Code(n) )
{

}

template <>
void CashKarp<real_t>::  operator()(const Readable<real_t> & y,
                                    const Readable<real_t> & dydx,
                                    const real_t             x,
                                    const real_t             h,
                                    Writable<real_t> &       yout,
                                    Writable<real_t> &       yerr,
                                    Equation     &           drvs,
                                    Callback     *           cntl)
{
    assert(y.size()==dydx.size());
    assert(y.size()==yout.size());
    assert(y.size()==yerr.size());

    const size_t n = y.size();
    if(0==code||code->dims<n)
    {
        Nullify(code);
        code = new Code(n);
    }
    assert(0!=code);
    return code->step(y,dydx,x,h, yout, yerr, drvs, cntl);
}
