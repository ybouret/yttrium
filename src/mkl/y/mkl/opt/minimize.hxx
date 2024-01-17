

template <>
real_t Minimize<real_t>:: Locate(Triplet<real_t> &x,
                                 Triplet<real_t> &f,
                                 FunctionType    &F)
{
    static const real_t zero(0);

    assert(x.isIncreasing());
    assert(f.isLocalMinimum());

    std::cerr << "Locate: init@" << x << " -> " << f << std::endl;
    real_t x_old = x.b; Parabolic<real_t>::Step(x,f,F);
    real_t x_new = x.b;
    real_t width = Fabs<real_t>::Of(x_new-x_old);
    std::cerr << "Locate: warm@" << x << " -> " << f << " / w=" << width << std::endl;

    if( Fabs<real_t>::Of(width) <= zero ) return x_new;

    x_old = x_new;
    while(true)
    {
        Parabolic<real_t>::Step(x,f,F);
        x_new                 = x.b;
        const real_t newWidth = Fabs<real_t>::Of(x_new-x_old);
        std::cerr << "Locate: loop@" << x << " -> " << f << " / w=" << newWidth << std::endl;

        if(newWidth <= zero || newWidth>=width)
        {
            return x_new;
        }
        x_old = x_new;
        width = newWidth;
    }

}

