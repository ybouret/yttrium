


template <>
TriDiag<real_t>:: ~TriDiag() noexcept { Nullify(code); }


template <>
TriDiag<real_t>:: TriDiag(const size_t n) :
code( new Code(n) ),
a(code->a),
b(code->b),
c(code->c)
{}




template <>
size_t TriDiag<real_t>:: size() const noexcept
{
    assert(0!=code);
    return code->size;
}

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
        case Positive: assert(i>j); return (1==i-j) ? a[i] : code->zero;
        case Negative: assert(i<j); return (1==j-i) ? c[i] : code->zero;
    }

    return b[i];
}
