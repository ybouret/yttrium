
template <> SVD<real_t>:: SVD() : code( new Code() ) { }

template <> SVD<real_t>:: ~SVD() noexcept
{
    assert(0!=code);
    Nullify(code);
}

template <> bool SVD<real_t>::build(Matrix<real_t> &a, Writable<real_t> &w, Matrix<real_t> &v)
{
    assert(0!=code);
    return code->build(a,w,v);
}
