
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
CashKarp<real_t>:: CashKarp(const size_t n) : code( new Code(n) )
{

}

template <>
void CashKarp<real_t>::  operator()(Writable<real_t> &       y,
                                    const Readable<real_t> & dydx,
                                    const real_t             x,
                                    const real_t             h,
                                    Writable<real_t> &       yout,
                                    Writable<real_t> &       yerr,
                                    Equation     &           drvs,
                                    Callback     *           cntl)
{
    const size_t n = y.size();
    if(0==code||code->dims<n)
    {
        delete code;
        code = 0;
        code = new Code(n);
    }
    assert(0!=code);
    return code->step(y,dydx,x,h, yout, yerr, drvs, cntl);
}
