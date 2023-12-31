
template <>
bool Bracket<real_t>:: Inside(Triplet<real_t>   &x,
                              Triplet<real_t>   &f,
                              FunctionType      &F)
{
    static const real_t half(0.5);
    //static const real_t zero(0);

    //--------------------------------------------------------------------------
    // order f.a<=f.c and initialize width
    //--------------------------------------------------------------------------
    if(f.a>f.c)
    {
        Swap(x.a,x.c);
        Swap(f.a,f.c);
    }
    real_t width = Fabs<real_t>::Of(x.c-x.a);

PROBE:
    assert(f.a<=f.c);
    real_t xmin = x.a;
    real_t xmax = x.c;
    if(xmin>xmax) Swap(xmin,xmax);
    f.b = F( x.b = Clamp(xmin,half*(xmin+xmax),xmax) );


    if(f.b <= f.a)
    {
        assert(f.isLocalMinimum());
        x.makeIncreasingWith(f);
        assert(x.isIncreasing());
        assert(f.isLocalMinimum());
        return true;
    }

    f.c = f.b;
    x.c = x.b;
    const real_t newWidth = Fabs<real_t>::Of(x.c-x.a);
    if(newWidth>=width)
    {
        f.c = f.b = f.a;
        x.c = x.b = x.a;
        return false; 
    }

    width = newWidth;
    goto PROBE;


}


template <>
void Bracket<real_t>:: Expand(Triplet<real_t>   &x,
                              Triplet<real_t>   &f,
                              FunctionType      &F)
{
    static const real_t lambda(1.2);
    
    // find decreasing direction
    if(f.b>f.a)
    {
        Swap(f.a,f.b);
        Swap(x.a,x.b);
    }

    assert(f.b<=f.a);
    // take a new step
PROBE:
    f.c = F(x.c = x.b + lambda * (x.b - x.a));
    if(f.b<=f.c)
    {
        x.makeIncreasingWith(f);
        return;
    }
    else
    {

        x.a = x.b;
        f.a = x.b;
        x.b = x.c;
        f.b = f.c;
        goto PROBE;
    }



}
