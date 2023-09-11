
template <> MidPoint<real_t>::  MidPoint() noexcept : code(0) {}

template <> MidPoint<real_t>:: ~MidPoint() noexcept { Nullify(code); }

template <> MidPoint<real_t>::  MidPoint(const size_t n):
code( n<=0 ? 0 : new Code(n) )
{}


template <>
void MidPoint<real_t>::  compute(const Readable<real_t> &y,
                                 const Readable<real_t> &dydx,
                                 const real_t            x,
                                 const real_t            h,
                                 const size_t            nstep,
                                 Writable<real_t> &      yout,
                                 Equation         &      drvs,
                                 Callback         *      cntl)
{
    assert(y.size()==dydx.size());
    assert(y.size()==yout.size());
    const size_t n = y.size();
    assert(n>0);
    if(0==code||code->dims<n)
    {
        Nullify(code);
        code = new Code(n);
    }
    code->compute(y, dydx, x, h, nstep, yout, drvs, cntl);
}
