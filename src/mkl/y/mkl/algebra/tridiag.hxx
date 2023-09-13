


template <>
TriDiag<real_t>:: ~TriDiag() noexcept { Nullify(code); }


template <>
TriDiag<real_t>:: TriDiag(const size_t n) :
code( new Code(n) ),
a(code->a),
b(code->b),
c(code->c),
size(code->size)
{}




template <>
real_t TriDiag<real_t>:: operator()(const size_t i, const size_t j) const
{
    assert(0!=code);
    assert(i>0);
    assert(j>0);
    assert(i<=code->size);
    assert(j<=code->size);

    switch( Sign::Of(i,j) )
    {
        case __Zero__: break;
        case Positive: assert(i>j); return (1==i-j) ? a[i] : code->t0;
        case Negative: assert(i<j); return (1==j-i) ? c[i] : code->t0;
    }

    return b[i];
}

template <>
bool  TriDiag<real_t>:: solve(Writable<real_t> &u, const Writable<real_t> &r)
{
    assert(0!=code);
    return code->solve(u,r);
}
