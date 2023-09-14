
template <>
Cyclic<real_t>:: ~Cyclic() noexcept
{
    Nullify(code);
}

template <>
Cyclic<real_t>:: Cyclic(const size_t n) :
TriDiag<real_t>( CheckCyclic(n) ),
code( new Code(n) ),
alpha( c[n] ),
beta(  a[1] )
{
}

template <>
real_t Cyclic<real_t>:: operator()(const size_t i, const size_t j) const
{
    assert(0!=code);
    assert(i>0);
    assert(j>0);
    assert(i<=size);
    assert(j<=size);

    if( (i==size) && (j==1)    ) return alpha;
    if( (i==1)    && (j==size) ) return beta;
    const TriDiag<real_t> &td = *this;
    return td(i,j);

}


template <>
bool  Cyclic<real_t>:: solve(Writable<real_t> &u, const Readable<real_t> &r)
{
    assert(0!=code);
    return code->solve(u,r,*this,alpha,beta);
}
