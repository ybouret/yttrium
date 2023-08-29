
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
