

template <> Diagonalization<real_t> :: Diagonalization() :
code( new Code() )
{

}

template <> Diagonalization<real_t> :: ~Diagonalization() noexcept
{
    assert(0!=code);
    Nullify(code);
}

template <> void Diagonalization<real_t> :: balance(Matrix<real_t> &a)
{
    assert(code);
    code->balance(a);
}

template <> void Diagonalization<real_t> :: reduce(Matrix<real_t> &a)
{
    assert(code);
    code->reduce(a);
}

template <> bool Diagonalization<real_t> ::  QR( Matrix<real_t> &a, Writable<real_t> &wr, Writable<real_t> &wi, size_t &nr)
{
    assert(0!=code);
    return code->QR(a,wr,wi,nr);
}

template <> const Values<real_t> *Diagonalization<real_t> :: eig( Matrix<real_t> &a )
{
    assert(0!=code);
    return code->eig(a);
}
