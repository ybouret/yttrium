
#include "y/chemical/reactive/aftermath.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Outcome:: Outcome(const Situation   _st,
                          const XReadable & _cc,
                          const Level     & _lv,
                          const xreal_t     _xi) noexcept :
        st(_st),
        cc(_cc),
        lv(_lv),
        xi(_xi)
        {
        }


        Outcome:: Outcome(const Outcome &_) noexcept :
        st(_.st),
        cc(_.cc),
        lv(_.lv),
        xi(_.xi)
        {}


        Outcome:: ~Outcome() noexcept
        {
        }



        namespace
        {
            struct Activator
            {
                const Components & E;
                const xreal_t      K;
                XWritable &        C;
                const Level        L;
                XMul              &xmul;
                XAdd              &xadd;

                inline xreal_t zp() const
                {
                    return E.activity(K,xmul,C,L);
                }

                inline xreal_t operator()(const xreal_t xi)
                {
                    return E.activity(K,xmul,C,L,xi);
                }


                void increase(XTriplet &xi, XTriplet &ff)
                {
                    Activator &F = *this;
                    while( (ff.c=F(xi.c)).mantissa > 0.0 ) xi.c += xi.c;
                }

            };


        }

        Aftermath:: Aftermath() noexcept
        {
        }

        Aftermath:: ~Aftermath() noexcept
        {
        }


        Outcome Aftermath:: operator()(const Components &E,
                                       const xreal_t     K,
                                       XWritable        &C,
                                       const Level       L,
                                       const XReadable  &C0,
                                       const Level       L0)
        {
            // prepare
            const Actors &reac = E.reac;
            const Actors &prod = E.prod; assert(reac->size>0||prod->size>0);
            Activator     F    = { E, K, C, L, xmul, xadd };




            const Situation st = E.situation(C,L);
            XTriplet        xi = { 0, 0, 0 };
            XTriplet        ff = { 0, 0, 0 };
            switch(E.kind)
            {
                case Deserted:
                    throw Specific::Exception("Aftermath", "Deserted '%s'", E.name->c_str());

                case ProdOnly:
                    assert(prod->size>0);
                    assert(reac->size<=0);

                    switch( Sign::Of(ff.a=F.zp() ) )
                    {
                        case __Zero__: return Outcome(st,C,L,0);
                        case Positive:
                            xi.c = prod.scaling(K);
                            F.increase(xi,ff);
                            break;
                        case Negative:
                            xi.c = -prod.limiting(C,L);
                            ff.c = K;
                            break;
                    }
                    break;

                case ReacOnly:
                case Standard:
                    throw Exception("Not Implemented");
            }

            std::cerr << "xi=" << xi << std::endl;
            std::cerr << "ff=" << ff << std::endl;

            return Outcome(Blocked,C, L, 0);


        }

    }

}
