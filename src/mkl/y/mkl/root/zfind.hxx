
template <> ZFind<real_t>::  ZFind() noexcept : half(0.5) {}
template <> ZFind<real_t>:: ~ZFind() noexcept {}

template <>
bool ZFind<real_t>:: found(Handle &xp, Handle &fp, Triplet<real_t> &x, Triplet<real_t> &f, FunctionType &F)
{
    
    assert(0==xp.pos);
    assert(0==xp.neg);
    assert(0==fp.neg);
    assert(0==fp.pos);

    switch( Sign::MakePairFrom(f.a,f.c) )
    {
        case ZP_Signs:
        case ZN_Signs:
            x.b = x.a;
            f.b = f.a = 0;
            return true;

        case PZ_Signs:
        case NZ_Signs:
            x.b = x.c;
            f.b = f.c = 0;
            return true;

        case NP_Signs:
            xp.neg = &x.a; fp.neg = &f.a;
            xp.pos = &x.c; fp.pos = &f.c;
            break;

        case PN_Signs:
            xp.neg = &x.c; fp.neg = &f.c;
            xp.pos = &x.a; fp.pos = &f.a;
            break;

        case ZZ_Signs:
            if( __Zero__ != Sign::Of( f.b = F( x.b = half*(x.a+x.c) ) ) )
                throw Specific::Exception( callSign(), "non zero-uniform function");
            return true;

        case NN_Signs:
        case PP_Signs:
            throw Specific::Exception( callSign(), "invalid starting point");
    }

    return false;

}
