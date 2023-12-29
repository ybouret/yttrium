
template <>
SmoothArc<real_t>:: SmoothArc() :
code( new Code() ),
r( code->r ),
v( code->v ),
curvature( code->curvature )
{
}

template <>
SmoothArc<real_t> :: ~SmoothArc() noexcept
{
    assert(0!=code);
    Nullify(code);
}

template <>
void SmoothArc<real_t> :: operator()(const real_t             &t0,
                                     const Readable<real_t>   &t,
                                     const Readable<real_t>   &x,
                                     const Readable<real_t>   &y,
                                     const size_t              xdegree,
                                     const size_t              ydegree,
                                     const SmoothGeometry      geometry)
{
    assert(0!=code);
    code->run(t0,t,x,y,xdegree,ydegree,geometry);
}


template <>
void SmoothArc<real_t> :: operator()(const real_t             &t0,
                                     const Readable<real_t>   &t,
                                     const Readable<Vtx>      &p,
                                     const size_t              xdegree,
                                     const size_t              ydegree,
                                     const SmoothGeometry      geometry)
{
    assert(0!=code);
    code->run(t0,t,p,xdegree,ydegree,geometry);
}
