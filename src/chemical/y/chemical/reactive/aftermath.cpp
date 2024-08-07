
#include "y/chemical/reactive/aftermath.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/triplet.hpp"

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

            //------------------------------------------------------------------
            //
            //
            //
            //! wrapper to have a callable object
            //
            //
            //
            //------------------------------------------------------------------
            struct CallMassAction
            {
                const XReadable &  C;
                Level              L;
                const Components & E;
                xreal_t            K;
                XMul              &xmul;

                inline xreal_t operator()(const xreal_t xi)
                {
                    return E.massAction(K, xmul, C, L, xi);
                }
            };

            //------------------------------------------------------------------
            //
            //
            //! solve once zero is bracketed
            //
            //
            //------------------------------------------------------------------
            static inline xreal_t xiSolve(Triplet<xreal_t> &xi,
                                          Triplet<xreal_t> &ma,
                                          const XReadable  &C,
                                          const Level      &L,
                                          const Components &E,
                                          const xreal_t     K,
                                          XMul             &xmul)
            {
                const xreal_t    half(0.5);
                CallMassAction   F  = { C, L, E, K, xmul };

                //--------------------------------------------------------------
                //
                // mass action is a decreasing function
                //
                //--------------------------------------------------------------
                if(xi.a>xi.c) {
                    Swap(xi.a,xi.c);
                    Swap(ma.a,ma.c);
                }

                //--------------------------------------------------------------
                //
                // check @a
                //
                //--------------------------------------------------------------
                switch( Sign::Of(ma.a) )
                {
                    case Negative: throw Specific::Exception(E.name.c_str(),"negative highest mass action");
                    case __Zero__: return xi.a;
                    case Positive: break;
                }

                //--------------------------------------------------------------
                //
                // check @c
                //
                //--------------------------------------------------------------
                switch( Sign::Of(ma.c) )
                {
                    case Positive: throw Specific::Exception(E.name.c_str(),"positive lowest mass action");
                    case __Zero__: return xi.c;
                    case Negative: break;
                }

                //--------------------------------------------------------------
                //
                // bissection up to numerical 0 or machine precision
                //
                //--------------------------------------------------------------
                assert(ma.a > xreal_t(0) );
                assert(ma.c < xreal_t(0) );
                xreal_t       width = (xi.c - xi.a).abs();
                for(;;)
                {
                    switch( Sign::Of( ma.b = F( xi.b = half*(xi.a+xi.c) ) ) )
                    {
                        case Negative: xi.c = xi.b; ma.c = ma.b; break;
                        case Positive: xi.a = xi.b; ma.a = ma.b; break;
                        case __Zero__: return xi.b;
                    }
                    const xreal_t newWidth = (xi.c - xi.a).abs();
                    if( newWidth >= width )
                    {
                        return xi.b;
                    }
                    width= newWidth;
                }

            }

            //__________________________________________________________________
            //
            //
            //! driver for ReacOnly components
            //
            //__________________________________________________________________
            static inline
            xreal_t xiReacOnly(const XReadable  &Cout,
                               const Level      &Lout,
                               const Components &E,
                               const xreal_t     K,
                               XMul             &xmul)
            {
                const xreal_t    zero;
                Triplet<xreal_t> xi = { zero, zero, zero };
                Triplet<xreal_t> ma = { E.massAction(K,xmul,Cout, Lout), zero, zero };


                switch( Sign::Of(ma.a) )
                {

                    case __Zero__:
                        //------------------------------------------------------
                        // numeric solution
                        //------------------------------------------------------
                        return zero;


                    case Negative:
                        //------------------------------------------------------
                        // must increase reactant(s) with negative xi
                        //------------------------------------------------------
                        xi.c = -K.pow(-E.reac.scale);
                        while( (ma.c = E.massAction(K,xmul,Cout,Lout,xi.c)) < zero )
                            ++Coerce(xi.c.exponent);
                        assert(ma.a<zero);
                        assert(ma.c>=zero);
                        break;


                    case Positive:
                        //------------------------------------------------------
                        // must decrease reactant(s) with positive xi
                        //------------------------------------------------------
                        xi.c   =  E.reac.maxExtent(Cout,Lout);
                        ma.c   = -1;

                        assert(ma.a>zero);
                        assert( E.massAction(K,xmul,Cout,Lout,xi.c) < zero );
                        break;
                }

                return xiSolve(xi, ma, Cout, Lout, E, K, xmul);
            }

            //__________________________________________________________________
            //
            //
            //! driver for ProdOnly components
            //
            //__________________________________________________________________
            static inline
            xreal_t xiProdOnly(const XReadable  &Cout,
                               const Level      &Lout,
                               const Components &E,
                               const xreal_t     K,
                               XMul             &xmul)
            {
                const xreal_t    zero;
                Triplet<xreal_t> xi = { zero, zero, zero };
                Triplet<xreal_t> ma = { E.massAction(K,xmul,Cout, Lout), zero, zero };

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
                        assert(ma.a<zero);
                        assert( E.massAction(K,xmul,Cout,Lout,xi.c) > zero);
                        break;


                    case Positive:
                        //------------------------------------------------------
                        // must increase product(s)
                        //------------------------------------------------------
                        xi.c = K.pow(E.prod.scale);
                        while( (ma.c = E.massAction(K,xmul,Cout,Lout,xi.c)) > zero )
                            ++Coerce(xi.c.exponent);
                        assert(ma.a>zero);
                        assert(ma.c<=zero);
                        break;
                }

                return xiSolve(xi, ma, Cout, Lout, E, K, xmul);
            }

            //------------------------------------------------------------------
            //
            //
            //! driver for tandard equilibrium
            //
            //
            //------------------------------------------------------------------
            static inline
            xreal_t xiStandard(const XReadable  &C,
                               const Level      &L,
                               const Components &E,
                               const xreal_t     K,
                               XMul             &xmul)
            {
                const xreal_t    zero;
                Triplet<xreal_t> xi = { zero, zero, zero };
                Triplet<xreal_t> ma = { E.massAction(K,xmul,C, L), zero, zero };

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
                        xi.c = -E.prod.maxExtent(C,L);        // => vanishing product
                        xmul.free(); xmul << K;               // => initialize @K
                        E.reac.massAction(xmul,C,L,-xi.c);    // => increase reactants (WARNING: sign)
                        ma.c = xmul.product();

                        assert(ma.a<zero);
                        assert(ma.c>=zero);
                        break;


                    case Positive:
                        //------------------------------------------------------
                        // must increase product(s)/decrease reactant(s) xi>0
                        //----------------------------------------------------------
                        xi.c = E.reac.maxExtent(C,L);          // => vanishing reactant
                        xmul.free(); xmul << E.mOne;           // => initialize @ -1
                        E.prod.massAction(xmul,C,L,xi.c);      // => increase product
                        ma.c = xmul.product();

                        assert(ma.a>zero);
                        assert(ma.c<=zero);
                        break;
                }

                
                return xiSolve(xi, ma, C, L, E, K, xmul);
            }

            //------------------------------------------------------------------
            //
            //
            //! driver to iterate search
            //
            //
            //------------------------------------------------------------------
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
                    const Situation ans = (E.prod.deficient(C,L) ? Crucial : Running);
                    solveWith(xiProdOnly,C,L,E,K,xmul);
                    assert( E.prod.accounted(C,L) );
                    return ans;
                }

                case ReacOnly: {
                    const Situation ans = (E.reac.deficient(C,L) ? Crucial : Running);
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
                    //----------------------------------------------------------
                    // deficient in both sides
                    //----------------------------------------------------------
                    return Blocked;
                }
                else
                {
                    //----------------------------------------------------------
                    // deficient in reac => crucial
                    //----------------------------------------------------------
                    assert(E.prod.accounted(C,L));
                    solveWith(xiStandard,C,L,E,K,xmul);
                    assert( E.prod.accounted(C,L) );
                    assert( E.reac.accounted(C,L) );
                    return Crucial;
                }
            }
            else
            {
                //--------------------------------------------------------------
                assert(E.reac.accounted(C,L));
                //--------------------------------------------------------------

                if(E.prod.deficient(C,L))
                {
                    //----------------------------------------------------------
                    // deficient in prod => crucial
                    //----------------------------------------------------------
                    solveWith(xiStandard,C,L,E,K,xmul);
                    assert( E.prod.accounted(C,L) );
                    assert( E.reac.accounted(C,L) );
                    return Crucial;
                }
                else
                {
                    //----------------------------------------------------------
                    // both sides are all accounted
                    //----------------------------------------------------------
                    solveWith(xiStandard,C,L,E,K,xmul);
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

