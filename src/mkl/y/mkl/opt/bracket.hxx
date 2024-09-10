
template <>
bool Bracket<real_t>:: Inside(Triplet<real_t>   &x,
                              Triplet<real_t>   &f,
                              FunctionType      &F)
{
    static const real_t half(0.5);
    Y_BRACKET_PRINT("<Inside x=" << x << " f=" << f <<">");

    //--------------------------------------------------------------------------
    // order f.a<=f.c and initialize width
    //--------------------------------------------------------------------------
    if(f.a>f.c)
    {
        Swap(x.a,x.c);
        Swap(f.a,f.c);
    }
    assert(f.a<=f.c);
    real_t width = Fabs<real_t>::Of(x.c-x.a);

PROBE:
    assert(f.a<=f.c);
    real_t xmin = x.a;
    real_t xmax = x.c;
    if(xmin>xmax) Swap(xmin,xmax);
    f.b = F( x.b = Clamp(xmin,half*(xmin+xmax),xmax) );

    Y_BRACKET_PRINT("probe @x=" << x << " f=" << f <<" width=" << width);


    if(f.b <= f.a)
    {
        assert(f.isLocalMinimum());
        assert(x.isOrdered());
        if(x.c<x.a)
        {
            Swap(x.a,x.c);
            Swap(f.a,f.c);
        }
        //x.makeIncreasingWith(f);
        //std::cerr << "x=" << x << std::endl;
        //std::cerr << "f=" << f << std::endl;
        assert(x.isIncreasing());
        assert(f.isLocalMinimum());
        Y_BRACKET_PRINT("found @x=" << x << " f=" << f);
        Y_BRACKET_PRINT("<Inside/>");
        return true;
    }

    f.c = f.b;
    x.c = x.b;
    const real_t newWidth = Fabs<real_t>::Of(x.c-x.a);
    if(newWidth>=width)
    {
        f.c = f.b = f.a;
        x.c = x.b = x.a;
        Y_BRACKET_PRINT("side  @x=" << x << " f=" << f);
        Y_BRACKET_PRINT("<Inside/>");
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
