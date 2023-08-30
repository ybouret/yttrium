
template <>
LU<real_t>:: LU() noexcept : code(0) {}

template <>
LU<real_t>:: ~LU() noexcept
{
    if(0!=code)
    {
        delete code;
        code = 0;
    }
}

template <>
LU<real_t>:: LU(const size_t nmin) :
code( (nmin > 0) ? new Code(nmin) : 0)
{}

template <>
void LU<real_t>:: ensure(const size_t nmin)
{
    if(nmin<=0) return;

    if(0==code)
    {
        code = new Code(nmin);
    }
    else
    {
        if(code->scal.size()<nmin)
        {
            delete code;
            code = 0;
            code = new Code(nmin);

        }
    }
    assert(0!=code);
    assert(code->scal.size()>=nmin);

}

template <>
bool LU<real_t>::build( Matrix<real_t> &a)
{
    assert(a.isSquare());
    assert(a.isValid());

    if(0==code)
    {
        code = new Code(a.rows);
    }
    else
    {
        if(code->scal.size()<a.rows)
        {
            delete code;
            code = 0;
            code = new Code(a.rows);
        }
    }
    assert(0!=code);
    assert(code->scal.size()>=a.rows);
    
    return code->build(a);
}

template<>
void LU<real_t>:: solve(const Matrix<real_t> &a, Writable<real_t> &b)
{
    assert(0!=code);
    code->solve(a,b);
}
