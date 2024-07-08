
#include "y/chemical/reactive/aftermath.hpp"
#include "y/mkl/root/zbis.hpp"
#include "y/mkl/root/zrid.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        using namespace MKL;

        Aftermath:: ~Aftermath() noexcept
        {
        }

        Aftermath:: Aftermath()  : xmul()
        {
        }


        struct CallMassAction
        {
            const XReadable &  C;
            Level              L;
            const Components & E;
            xreal_t            K;
            XMul              &xmul;

            inline xreal_t operator()(const xreal_t xi)
            {
                return E.massAction(K, xmul, C, xi, L);
            }
        };




        static inline
        xreal_t xiProdOnly(XWritable        &Cout,
                           const Level      &Lout,
                           const Components &E,
                           const xreal_t     K,
                           const xreal_t     Cs,
                           XMul             &xmul)
        {
            Triplet<xreal_t> xi = { 0, 0, 0 };
            Triplet<xreal_t> ma = { E.massAction(K,xmul,Cout, Lout), 0, 0 };
            switch( Sign::Of(ma.a) )
            {
                case __Zero__: // numeric solution
                    return 0;
                case Negative: // must decrease product(s)
                    std::cerr << "Xi<0" << std::endl;
                    xi.c = - E.prod.maxExtent(Cout,Lout);
                    ma.c =   K; // E.massAction(K,xmul,Cout, xi.c, Lout),
                    break;

                case Positive: // must increase product(s)
                    std::cerr << "Xi>0" << std::endl;

            }
            CallMassAction   F  = { Cout, Lout, E, K, xmul };
            ZBis<xreal_t>    zroot;
            zroot(F,xi,ma);

            return xi.b;
        }

#if 0
        bool Aftermath:: solveProdOnly(XWritable        &Cout,
                                       const Level      &Lout,
                                       const Components &E,
                                       const xreal_t     K)
        {
            std::cerr << "SolveProdOnly" << std::endl;
            Triplet<xreal_t> xi = { 0, 0, 0 };
            Triplet<xreal_t> ma = { E.massAction(K,xmul,Cout, Lout), 0, 0 };
            switch( Sign::Of(ma.a) )
            {
                case __Zero__: return true;
                case Negative: // must decrease prpduct
                    std::cerr << "Xi<0" << std::endl;
                    xi.c = - E.prod.maxExtent(Cout,Lout);
                    ma.c =   K; // E.massAction(K,xmul,Cout, xi.c, Lout),
                    //std::cerr << "xi=" << xi << " -> " << ma << "/" << K << std::endl;
                    break;

                case Positive: // positive
                    std::cerr << "Xi>0" << std::endl;
                    break;

            }
            CallMassAction   F  = { Cout, Lout, E, K, xmul };
            ZBis<xreal_t>    zroot;
            zroot(F,xi,ma);


            return false;

        }
#endif

        bool Aftermath:: solve(XWritable       &  Cout,
                               const Level        Lout,
                               const XReadable &  Cinp,
                               const Level        Linp,
                               const Components & E,
                               const xreal_t      K)
        {


            switch(E.kind)
            {
                case Components::Nebulous: break;
                case Components::ProdOnly:
                    E.transfer(Cout,Lout,Cinp,Linp);
                    break;

                case Components::ReacOnly:
                case Components::Standard:

                    break;
            }




            return false;
        }

    }

}

