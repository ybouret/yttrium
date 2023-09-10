

template <>
RK45<real_t>:: RK45() noexcept : code(0)
{
}


template <>
RK45<real_t>:: RK45(const size_t n): code( n <=0 ? 0 : new Code(n) )
{
}


template <>
RK45<real_t>:: ~RK45() noexcept
{
    Nullify(code);
}
