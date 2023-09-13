
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

