
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

        Aftermath:: Aftermath()  : xmul(), xadd()
        {
        }


        namespace
        {
            static unsigned numCalls = 0;

            //! wrapper to have a callable object
            struct CallMassAction
            {
                const XReadable &  C;
                Level              L;
                const Components & E;
                xreal_t            K;
                XMul              &xmul;

                inline xreal_t operator()(const xreal_t xi)
                {
                    ++numCalls;
                    return E.massAction(K, xmul, C, L, xi);
                }
            };

            //! solve once zero is bracketed
            static inline xreal_t xiSolve(Triplet<xreal_t> &xi,
                                          Triplet<xreal_t> &ma,
                                          const XReadable  &Cout,
                                          const Level      &Lout,
                                          const Components &E,
                                          const xreal_t     K,
                                          XMul             &xmul)
            {
                CallMassAction   F  = { Cout, Lout, E, K, xmul };
                ZBis<xreal_t>    zroot;
                //ZRid<xreal_t>    zroot;

                zroot(F,xi,ma);
                //std::cerr << "#calls=" << numCalls << std::endl;
                return xi.b;
            }

            //! driver for ReacOnly components
            static inline
            xreal_t xiReacOnly(const XReadable  &Cout,
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

                    case __Zero__:
                        //------------------------------------------------------
                        // numeric solution
                        //------------------------------------------------------
                        return 0;


                    case Negative:
                        //------------------------------------------------------
                        // must increase reactant(s) with negative xi
                        //------------------------------------------------------
                        xi.c = -K.pow(-E.reac.scale);
                        while( (ma.c = E.massAction(K,xmul,Cout,Lout,xi.c)) < zero )
                        {
                            ++Coerce(xi.c.exponent);
                        }
                        break;


                    case Positive:
                        //------------------------------------------------------
                        // must decrease reactant(s) with positive xi
                        //------------------------------------------------------
                        xi.c   =  E.reac.maxExtent(Cout,Lout);
                        ma.c   = -1;
                        //std::cerr << E.massAction(K,xmul,Cout, xi.c, Lout) << std::endl;
                        break;
                }

                return xiSolve(xi, ma, Cout, Lout, E, K, xmul);
            }


            //! driver for ProdOnly components
            static inline
            xreal_t xiProdOnly(const XReadable  &Cout,
                               const Level      &Lout,
                               const Components &E,
                               const xreal_t     K,
                               XMul             &xmul)
            {
                const xreal_t    zero;
                Triplet<xreal_t> xi   = { 0, 0, 0 };
                Triplet<xreal_t> ma   = { E.massAction(K,xmul,Cout, Lout), 0, 0 };

                switch( Sign::Of(ma.a) )
                {
                    case __Zero__:
                        //------------------------------------------------------
                        // numeric solution
                        //------------------------------------------------------
                        return 0;

                    case Negative:
                        //------------------------------------------------------
                        // must decrease product(s)
                        //------------------------------------------------------
                        xi.c = - E.prod.maxExtent(Cout,Lout);
                        ma.c =   K;
                        break;


                    case Positive:
                        //------------------------------------------------------
                        // must increase product(s)
                        //------------------------------------------------------
                        xi.c = K.pow(E.prod.scale);
                        while( (ma.c = E.massAction(K,xmul,Cout,Lout,xi.c)) > zero )
                        {
                            ++Coerce(xi.c.exponent);
                        }
                        break;
                }

                return xiSolve(xi, ma, Cout, Lout, E, K, xmul);
            }

            static inline
            xreal_t xiStandard(const XReadable  &Cout,
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
                    case __Zero__:
                        //------------------------------------------------------
                        // numeric solution
                        //------------------------------------------------------
                        return 0;

                    case Negative:
                        //------------------------------------------------------
                        // must decrease product(s)/increase reactant(s) xi<0
                        //------------------------------------------------------
                        xi.c = -E.prod.maxExtent(Cout, Lout);
                        ma.c = E.massAction(K,xmul,Cout,Lout,xi.c);
                        break;


                    case Positive:
                        //------------------------------------------------------
                        // must increase product(s)/decrease reactant(s) xi>0
                        //----------------------------------------------------------
                        xi.c = E.reac.maxExtent(Cout, Lout);
                        ma.c = E.massAction(K,xmul,Cout,Lout,xi.c);
                        break;
                }

                return xiSolve(xi, ma, Cout, Lout, E, K, xmul);
            }

            //! driver to iterate search
            template <typename XI_PROC> static inline
            void solveWith(XI_PROC          &xiProc,
                           XWritable        &Cout,
                           const Level      &Lout,
                           const Components &E,
                           const xreal_t     K,
                           XMul             &xmul)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                xreal_t xi = xiProc(Cout, Lout, E, K, xmul);
                E.moveSafe(Cout,Lout,xi);


                xreal_t ax = xi.abs();
                if(ax.mantissa<=0) return;


                //--------------------------------------------------------------
                //
                // improve
                //
                //--------------------------------------------------------------
            IMPROVE:
                {
                    const xreal_t xi_new = xiProc(Cout, Lout, E, K, xmul);
                    E.moveSafe(Cout,Lout,xi_new);

                    const xreal_t ax_new = xi_new.abs();
                    if( ax_new.mantissa <= 0 ||  ax_new >= ax )
                    {
                        return;
                    }
                    xi = xi_new;
                    ax = ax_new;
                    goto IMPROVE;
                }
            }


        }




        Situation Aftermath::seek(XWritable        &C,
                                  const Level       L,
                                  const Components &E,
                                  const xreal_t     K)
        {
            switch(E.kind)
            {
                case Nebulous: 
                    return Blocked;

                case ProdOnly:  {
                    const Situation ans = E.prod.deficient(C,L) ? Crucial : Running;
                    solveWith(xiProdOnly,C,L,E,K,xmul);
                    assert( E.prod.accounted(C,L) );
                    return ans;
                }

                case ReacOnly: {
                    const Situation ans = E.reac.deficient(C,L) ? Crucial : Running;
                    solveWith(xiReacOnly,C,L,E,K,xmul);
                    assert( E.prod.accounted(C,L) );
                    return ans;
                }

                case Standard: 
                    break;
            }
            
            assert(Standard==E.kind);
            if( E.reac.deficient(C,L) )
            {
                if(E.prod.deficient(C,L))
                {
                    // deficient in both sides
                    return Blocked;
                }
                else
                {
                    // deficient in reac => crucial
                    solveWith(xiStandard,C,L,E,K,xmul);
                    assert( E.prod.accounted(C,L) );
                    assert( E.reac.accounted(C,L) );
                    return Crucial;
                }
            }
            else
            {
                assert(E.reac.accounted(C,L));
                solveWith(xiStandard,C,L,E,K,xmul);
                if(E.prod.deficient(C,L))
                {
                    // deficient in prod => crucial
                    assert( E.prod.accounted(C,L) );
                    assert( E.reac.accounted(C,L) );
                    return Crucial;
                }
                else
                {
                    // both sides are all accounted
                    assert( E.prod.accounted(C,L) );
                    assert( E.reac.accounted(C,L) );
                    return Running;
                }
            }
        }


        xreal_t Aftermath:: eval(XWritable       &  dOut,
                                 const XReadable &  Cout,
                                 const Level        Lout,
                                 const XReadable &  Cinp,
                                 const Level        Linp,
                                 const Components & E)
        {

            xadd.free();
            dOut.ld(0);
            Components::ConstIterator i = E->begin();
            const size_t              m = E->size();

            for(size_t j = m;j>0;--j,++i)
            {
                const Component &cc   = **i;
                const Species   &sp   = cc.sp;
                const size_t     iout = sp.indx[Lout];
                const xreal_t    cnew = Cout[iout];
                const xreal_t    cold = Cinp[sp.indx[Linp]];
                const xreal_t    xi   = (dOut[iout] = (cnew-cold) )/cc.xn;
                xadd << xi;
            }

            return xadd.sum()/xreal_t(m);
        }
        
        xreal_t Aftermath:: eval(const XReadable &  Cout,
                                 const Level        Lout,
                                 const XReadable &  Cinp,
                                 const Level        Linp,
                                 const Components & E)
        {

            xadd.free();
            Components::ConstIterator i = E->begin();
            const size_t              m = E->size();

            for(size_t j = m;j>0;--j,++i)
            {
                const Component &cc   = **i;
                const Species   &sp   = cc.sp;
                const size_t     iout = sp.indx[Lout];
                const xreal_t    cnew = Cout[iout];
                const xreal_t    cold = Cinp[sp.indx[Linp]];
                const xreal_t    xi   =  (cnew-cold)/cc.xn;
                xadd << xi;
            }

            return xadd.sum()/xreal_t(m);
        }

    }

}

