
#include "y/chemical/reactive/aftermath.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/root/zbis.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        


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


                void increaseXi(XTriplet &xi, XTriplet &ff)
                {
                    assert(xi.c>0.0);
                    Activator &F = *this;
                    while( (ff.c=F(xi.c)).mantissa > 0.0 ) xi.c += xi.c;
                }

                void decreaseXi(XTriplet &xi, XTriplet &ff)
                {
                    assert(xi.c<0.0);
                    Activator &F = *this;
                    while( (ff.c=F(xi.c)).mantissa < 0.0 ) xi.c += xi.c;
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
                case Deserted: throw Specific::Exception("Aftermath", "Deserted '%s'", E.name->c_str());


                case ProdOnly:
                    //----------------------------------------------------------
                    //
                    // Handling ProdOnly
                    //
                    //----------------------------------------------------------
                    assert(prod->size>0);
                    assert(reac->size<=0);

                    switch( Sign::Of(ff.a) )
                    {
                        case __Zero__: return zero;
                        case Positive:
                            xi.c = K.pow(prod.kxp); //prod.scaling(K);
                            F.increaseXi(xi,ff);
                            break;
                        case Negative:
                            xi.c = -prod.limiting(C,L);
                            ff.c = K;
                            break;
                    }
                    break;

                case ReacOnly:
                    //----------------------------------------------------------
                    //
                    // Handling ProdOnly
                    //
                    //----------------------------------------------------------
                    assert(prod->size<=0);
                    assert(reac->size>0);

                    switch( Sign::Of(ff.a) )
                    {
                        case __Zero__: return zero;
                        case Positive:
                            xi.c = reac.limiting(C,L);
                            ff.c = -1;
                            break;

                        case Negative:
                            xi.c = -K.pow(-reac.kxp);
                            F.decreaseXi(xi,ff);
                            break;
                    }
                    break;

                case Standard:
                    //----------------------------------------------------------
                    //
                    // Handling Standard
                    //
                    //----------------------------------------------------------
                    assert(prod->size>0);
                    assert(reac->size>0);
                    switch( Sign::Of(ff.a) )
                    {
                        case __Zero__: return zero;
                        case Positive: ff.c = F( xi.c =  reac.limiting(C,L) ); break;
                        case Negative: ff.c = F( xi.c = -prod.limiting(C,L) ); break;
                    }
                    break;
            }

            //Core::Display(std::cerr << "xi=", &xi.a, 3, xreal_t::ToString) << std::endl;
            //Core::Display(std::cerr << "ff=", &ff.a, 3, xreal_t::ToString) << std::endl;

            root(F,xi,ff);
            //std::cerr << real_t(ff.c) << " @" << real_t(xi.c) << std::endl;
            const xreal_t x = xi.c;
            E.safeMove(C,L,xi.c);
            return xi.c;

        }

        Outcome Aftermath:: operator()(const Equilibrium & E,
                                       const xreal_t       K,
                                       XWritable &         C,
                                       const Level         L,
                                       const XReadable &   C0,
                                       const Level         L0)
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
                return Outcome(Blocked,E,K,C,L,0);


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
            return Outcome(st, E, K, C, L, E.extent(xadd,C,L,C0,L0));
        }

    }

}
