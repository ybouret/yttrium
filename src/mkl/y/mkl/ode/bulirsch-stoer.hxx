
template <>
BulirschStoer<real_t>::BulirschStoer() noexcept : code(0) {}

template <>
BulirschStoer<real_t>:: ~BulirschStoer() noexcept
{
    Nullify(code);
}
