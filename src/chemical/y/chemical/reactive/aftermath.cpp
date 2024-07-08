
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
        xreal_t xiReacOnly(XWritable        &Cout,
                           const Level      &Lout,
                           const Components &E,
                           const xreal_t     K,
                           XMul             &xmul)
        {
            const xreal_t    zero = 0;
            Triplet<xreal_t> xi   = { 0, 0, 0 };
            Triplet<xreal_t> ma   = { E.massAction(K,xmul,Cout, Lout), 0, 0 };

            std::cerr << "ma=" << ma.a << std::endl;

            switch( Sign::Of(ma.a) )
            {
                    //----------------------------------------------------------
                case __Zero__: // numeric solution
                    //----------------------------------------------------------
                    return 0;

                    //----------------------------------------------------------
                case Negative: // must increase reactant(s) with negative xi
                    //----------------------------------------------------------
                    std::cerr << "ReacOnly Xi<0" << std::endl;
                    xi.c = -K.pow(-E.reac.scale);
                    while( (ma.c = E.massAction(K,xmul,Cout, xi.c, Lout)) < zero )
                    {
                        ++Coerce(xi.c.exponent);
                    }
                    break;

                    //----------------------------------------------------------
                case Positive: // must decrease reactant(s) with positive xi
                    //----------------------------------------------------------
                    std::cerr << "ReacOnly Xi>0" << std::endl;
                    xi.c   =  E.reac.maxExtent(Cout,Lout);
                    ma.c   = -1;
                    std::cerr << E.massAction(K,xmul,Cout, xi.c, Lout) << std::endl;
                    break;
            }


            CallMassAction   F  = { Cout, Lout, E, K, xmul };
            ZBis<xreal_t>    zroot;
            zroot(F,xi,ma);

            return xi.b;
        }


        static inline
        xreal_t xiProdOnly(XWritable        &Cout,
                           const Level      &Lout,
                           const Components &E,
                           const xreal_t     K,
                           XMul             &xmul)
        {
            const xreal_t    zero = 0;
            Triplet<xreal_t> xi   = { 0, 0, 0 };
            Triplet<xreal_t> ma   = { E.massAction(K,xmul,Cout, Lout), 0, 0 };

            switch( Sign::Of(ma.a) )
            {
                    //----------------------------------------------------------
                case __Zero__: // numeric solution
                    //----------------------------------------------------------
                    return 0;

                    //----------------------------------------------------------
                case Negative: // must decrease product(s)
                    //----------------------------------------------------------
                    xi.c = - E.prod.maxExtent(Cout,Lout);
                    ma.c =   K;
                    break;

                    //----------------------------------------------------------
                case Positive: // must increase product(s)
                    //----------------------------------------------------------
                    //std::cerr << "Xi>0" << std::endl;
                    xi.c = K.pow(E.prod.scale);
                    while( (ma.c = E.massAction(K,xmul,Cout, xi.c, Lout)) > zero )
                    {
                        ++Coerce(xi.c.exponent);
                    }
                    break;
            }

            CallMassAction   F  = { Cout, Lout, E, K, xmul };
            ZBis<xreal_t>    zroot;
            zroot(F,xi,ma);

            return xi.b;
        }

        // driver to iterate search
        template <typename XI_PROC> static inline
        bool solveWith(XI_PROC &xiProc,
                       XWritable        &Cout,
                       const Level      &Lout,
                       const Components &E,
                       const xreal_t     K,
                       XMul             &xmul)
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            xreal_t xi = xiProc(Cout, Lout, E, K, xmul);
            E.moveSave(Cout,xi,Lout);

            xreal_t ax = xi.abs();
            if(ax.mantissa<=0) return true;

            //------------------------------------------------------------------
            //
            // improve
            //
            //------------------------------------------------------------------
        IMPROVE:
            {
                const xreal_t xi_new = xiProc(Cout, Lout, E, K, xmul);
                E.moveSave(Cout,xi_new,Lout);

                const xreal_t ax_new = xi_new.abs();
                if( ax_new.mantissa <= 0 ||  ax_new >= ax )
                {
                    return true;
                }
                xi = xi_new;
                ax = ax_new;
                goto IMPROVE;
            }
        }

#if 0
        static inline
        bool solveProdOnly(XWritable        &Cout,
                           const Level      &Lout,
                           const Components &E,
                           const xreal_t     K,
                           XMul             &xmul)
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            xreal_t xi = xiProdOnly(Cout, Lout, E, K, xmul);
            E.moveSave(Cout,xi,Lout);

            xreal_t ax = xi.abs();
            if(ax.mantissa<=0) return true;

            //------------------------------------------------------------------
            //
            // improve
            //
            //------------------------------------------------------------------
        IMPROVE:
            {
                const xreal_t xi_new = xiProdOnly(Cout, Lout, E, K, xmul);
                E.moveSave(Cout,xi_new,Lout);

                const xreal_t ax_new = xi_new.abs();
                if( ax_new.mantissa <= 0 ||  ax_new >= ax )
                {
                    return true;
                }
                xi = xi_new;
                ax = ax_new;
                goto IMPROVE;
            }

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
                    return solveWith(xiProdOnly,Cout,Lout,E,K,xmul);

                case Components::ReacOnly:
                    E.transfer(Cout,Lout,Cinp,Linp);
                    return solveWith(xiReacOnly,Cout,Lout,E,K,xmul);

                case Components::Standard:

                    break;
            }




            return false;
        }

    }

}

