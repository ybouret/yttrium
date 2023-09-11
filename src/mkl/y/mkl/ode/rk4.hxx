
template <>
RK4<real_t>:: RK4(const size_t dims) :
code( dims > 0 ? new Code(dims) : 0 )
{
}

template <>
RK4<real_t>:: RK4() noexcept : code(0) {}


template <>
RK4<real_t>:: ~RK4() noexcept
{
    Nullify(code);
}

template <>
void RK4<real_t>:: operator()(Writable<real_t> &y,
                              Equation         &eqs,
                              const real_t     t0,
                              const real_t     t1,
                              Callback        *cb)
{
    const size_t nvar = y.size(); assert(nvar>0);
    if(0==code||nvar!=code->nvar)
    {
        Nullify(code);
        code = new Code(nvar);
    }
    code->step(y,eqs,t0,t1,cb);
}
