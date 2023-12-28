
template <>
SavGol<real_t> :: SavGol(const SavGolFactory &factory) :
code( new Code(factory) ),
xadd( code->add )
{
}

template <>
SavGol<real_t> :: ~SavGol() noexcept
{
    assert(0!=code);
    Nullify(code);
}

template <>
const Matrix<real_t> & SavGol<real_t>:: fetch(const uint32_t nLeft,
                                              const uint32_t nRight,
                                              const uint32_t degree)
{
    assert(0!=code);
    const int32_t j = nLeft+1;
    const int32_t n = j+nRight;
    if( degree >= uint32_t(n) ) throw Exception("SavGol<%s> degree=%u >= %d points", code->tid.c_str(), degree, n);
    return code->get(n,j,degree).f;
}

template <>
const Matrix<real_t> & SavGol<real_t>:: get(const size_t  n,
                                            const size_t  j,
                                            const size_t  d)
{
    assert(0!=code);
    if( d >= n ) throw Exception("SavGol<%s> degree=%u >= %u points", code->tid.c_str(), unsigned(d), unsigned(n));
    return code->get(static_cast<int32_t >(n),
                     static_cast<int32_t >(j),
                     static_cast<uint32_t>(d)).f;
}
