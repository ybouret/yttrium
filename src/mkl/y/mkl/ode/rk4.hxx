
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
    if(code)
    {
        delete code;
        code = 0;
    }
}

template <>
void RK4<real_t>:: operator()(Writable<real_t> &y,
                              Equation         &eqs,
                              const real_t     t0,
                              const real_t     t1)
{
    const size_t nvar = y.size(); assert(nvar>0);
    if(0==code||nvar!=code->nvar)
    {
        if(code) { delete code; code=0; }
        code = new Code(nvar);
    }
    code->step(y,eqs,t0,t1);

}
