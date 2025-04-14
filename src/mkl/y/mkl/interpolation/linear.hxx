template <>
LinearInterpolation<real_t>::  LinearInterpolation() noexcept {}

template <>
LinearInterpolation<real_t>:: ~LinearInterpolation() noexcept {}



template <>
real_t LinearInterpolation<real_t>:: operator()(const real_t            x,
                                                const Readable<real_t> &xa,
                                                const Readable<real_t> &ya) const
{
    assert(ya.size()==xa.size());
    const size_t n = xa.size();
    switch(n)
    {
        case 0: return 0;
        case 1: return ya[1];
        default: break;
    }

    if(x<=xa[1]) return ya[1];
    if(x>=xa[n]) return ya[n];

    size_t jlo = 1;
    size_t jup = n;
    while(jup-jlo>1)
    {
        const size_t jmd = (jlo+jup)>>1;
        const real_t xmd = xa[jmd];
        switch( Sign::Of(x,xmd) )
        {
            case __Zero__: return ya[jmd];
            case Negative: jup = jmd; continue;
            case Positive: jlo = jmd; continue;
        }
    }
    assert(jlo<n);
    assert(x>=xa[jlo]);
    jup=jlo+1;

    const real_t xlo = xa[jlo];
    const real_t xup = xa[jup];
    const real_t ylo = ya[jlo];
    const real_t yup = ya[jup];

    return ylo + (x-xlo) * (yup-ylo) / (xup-xlo);

}
