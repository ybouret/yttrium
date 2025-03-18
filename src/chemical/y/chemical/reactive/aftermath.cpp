
#include "y/chemical/reactive/aftermath.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/root/zbis.hpp"

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

        Aftermath:: Aftermath() noexcept :
        xmul(),
        xadd(),
        root(),
        zero(0)
        {
        }

        Aftermath:: ~Aftermath() noexcept
        {
        }


        xreal_t Aftermath:: improve(const Components &E,
                                    const xreal_t     K,
                                    XWritable        &C,
                                    const Level       L)
        {

            //------------------------------------------------------------------
            //
            //
            // fetch information
            //
            //
            //------------------------------------------------------------------
            assert( Blocked!= E.situation(C,L) );
            const Actors &  reac = E.reac;
            const Actors &  prod = E.prod; assert(reac->size>0||prod->size>0);

            //------------------------------------------------------------------
            //
            //
            // prepare solver
            //
            //
            //------------------------------------------------------------------
            Activator       F    = { E, K, C, L, xmul, xadd };
            XTriplet        xi   = { zero,   zero, zero };
            XTriplet        ff   = { F.zp(), zero, zero };

            //------------------------------------------------------------------
            //
            //
            // initialize look-up
            //
            //
            //------------------------------------------------------------------
            switch(E.kind)
            {
                case Deserted:
                    throw Specific::Exception("Aftermath", "Deserted '%s'", E.name->c_str());

                case ProdOnly:
                    assert(prod->size>0);
                    assert(reac->size<=0);

                    switch( Sign::Of(ff.a) )
                    {
                        case __Zero__: return zero;
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

            Core::Display(std::cerr << "xi=", &xi.a, 3, xreal_t::ToString) << std::endl;
            Core::Display(std::cerr << "ff=", &ff.a, 3, xreal_t::ToString) << std::endl;

            root(F,xi,ff);
            std::cerr << real_t(ff.c) << " @" << real_t(xi.c) << std::endl;

            const xreal_t x = xi.c;
            E.moveSafely(C,L,xi.c);
            return xi.c;

        }

        Outcome Aftermath:: operator()(const Components &E,
                                       const xreal_t     K,
                                       XWritable        &C,
                                       const Level       L,
                                       const XReadable  &C0,
                                       const Level       L0)
        {
            //------------------------------------------------------------------
            //
            //
            // query current situation
            //
            //
            //------------------------------------------------------------------
            const Situation st = E.situation(C,L);
            if(Blocked==st)
                return Outcome(Blocked,C,L,0);


            //------------------------------------------------------------------
            //
            //
            // Iterative solution
            //
            //
            //------------------------------------------------------------------
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                xreal_t         xi = improve(E, K, C, L);

                //--------------------------------------------------------------
                //
                // iterative improvement
                //
                //--------------------------------------------------------------
                if( xi.abs() > zero)
                {
                    while(true) {
                        const xreal_t xiNew = improve(E,K,C,L);
                        if( xiNew >= xi || xiNew.abs() <= zero)
                            break;
                        xi = xiNew;
                    }
                }
            }

            //------------------------------------------------------------------
            //
            //
            // evaluate xi by average increase
            //
            //------------------------------------------------------------------
            xadd.free();
            const size_t n = E->size();
            {
                size_t j = n;
                for(Components::ConstIterator it=E->begin();j-- > 0;++it)
                {
                    const Actor   &ac = **it;
                    const Species &sp = ac.sp;
                    const xreal_t  dc = (sp(C,L) - sp(C0,L0))/ac.xn;
                    xadd << dc;
                }
                assert(xadd.size()==n);
            }

            const xreal_t denom = n;
            const xreal_t xi    = xadd.sum() / denom;
            std::cerr << "xi=" << double(xi) << "; C=" << C << std::endl;
            return Outcome(st, C, L, xadd.sum()/denom);
        }

    }

}
