
template <> Jacobi<real_t>:: ~Jacobi() noexcept
{
    assert(0!=code);
    Nullify(code);
}

template <> Jacobi<real_t>:: Jacobi() :
code( new Code() )
{
    std::cerr << "sizeof(Code)=" << sizeof(Code) << std::endl;
}

template <>
bool Jacobi<real_t>:: build(Matrix<real_t> &a, Writable<real_t> &d, Matrix<real_t> &v)
{
    assert(0!=code);
    return code->process(a,d,v);
}
