
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


template<>
void LU<real_t>:: solve(const Matrix<real_t> &a, Matrix<real_t> &b)
{
    assert(0!=code);
    code->solve(a,b);
}

template<>
void LU<real_t>:: invert(const Matrix<real_t> &a, Matrix<real_t> &inv)
{
    assert(0!=code);
    code->invert(a,inv);
}

template <>
real_t  LU<real_t>:: determinant(const Matrix<real_t> &a)
{
    assert(0!=code);
    return code->det(a);
}


template <>
Writable<real_t> &  LU<real_t>:: getAux() noexcept
{
    assert(0!=code);
    return code->aux;
}


template <>
Antelope::Mul<real_t> &  LU<real_t>:: xmul() noexcept
{
    assert(0!=code);
    return code->xmul;
}


template <>
Antelope::Add<real_t> &  LU<real_t>:: xadd() noexcept
{
    assert(0!=code);
    return code->xadd;
}

template <>
void LU<real_t>:: adjoint(Matrix<real_t>       &adj,
                          const Matrix<real_t> &a)
{
    assert(a.isSquare());
    assert(a.isValid());
    assert(adj.hasSameMetricsThan(a));
    if(a.rows>1)
    {
        ensure(a.rows-1);
        code->ajdoint(adj,a);
    }
    else
    {
        const real_t one(1);
        adj[1][1] = one;
    }
}
