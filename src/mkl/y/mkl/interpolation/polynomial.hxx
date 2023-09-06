
template <>
PolynomialInterpolation<real_t>:: PolynomialInterpolation() noexcept : code(0) {}

template <>
PolynomialInterpolation<real_t>:: PolynomialInterpolation(const size_t n) :
code(n>0 ? new Code(n) : 0)
{}



template <>
PolynomialInterpolation<real_t>:: ~PolynomialInterpolation() noexcept
{
    if(0!=code)
    {
        delete code;
        code = 0;
    }
}

template <>
real_t PolynomialInterpolation<real_t>:: operator()(const real_t            x,
                                                    const Readable<real_t> &xa,
                                                    const Readable<real_t> &ya,
                                                    real_t                 &dy)
{
    static const real_t zero(0);
    assert(xa.size()==ya.size());
    const size_t n = xa.size();
    if(n<=0)
    {
        dy = zero;
        return zero;
    }
    else
    {
        if(0==code)
        {
            code = new Code(n);
        }
        else
        {
            if(code->c.size()<n)
            {
                Code *newCode = new Code(n);
                delete code;
                code = newCode;
            }
        }
        assert(0!=code);
        assert(code->c.size()>=n);
        return code->compute(x,xa,ya,dy);
    }
}



