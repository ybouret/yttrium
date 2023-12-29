
template <>
Smooth<real_t>:: ~Smooth() noexcept
{
    assert(0!=code);
    Nullify(code);
}

template <>
Smooth<real_t>:: Smooth() :
code( new Code() )
{
}


template <>
void Smooth<real_t>:: operator()(const real_t           &t0,
                                 const Readable<real_t> &t,
                                 const Readable<real_t> &z,
                                 const size_t            degree)
{
    assert(0!=code);
    code->run(t0,t,z,degree);
}

template <>
const char * Smooth<real_t>:: callSign() const noexcept
{
    return "Smooth";
}

template <>
size_t Smooth<real_t>:: size() const noexcept
{
    return 3;
}

template <>
const real_t & Smooth<real_t>:: operator[](const size_t idx) const noexcept
{
    assert(0!=code);
    assert(idx>=1);
    assert(idx<=3);
    return code->coef[idx];
}


template <>
void Smooth<real_t>:: reserveMaxDegree(const size_t degree)
{
    assert(0!=code);
    code->reserveMaxDegree(degree);
}

template <>
void Smooth<real_t>:: reserveMaxLength(const size_t points)
{
    assert(0!=code);
    code->reserveMaxLength(points);
}
