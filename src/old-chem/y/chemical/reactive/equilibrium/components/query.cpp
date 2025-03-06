#include "y/chemical/reactive/equilibrium/components.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        xReal Components:: activity(XMul &xmul, const xReal K, const XReadable &C, const Level L) const
        {
            xmul.free();
            xmul << K;
            reac.activity(xmul,C,L);
            const xReal ra = xmul.product();

            assert(xmul.isEmpty());
            xmul << one;
            prod.activity(xmul,C,L);
            const xReal pa = xmul.product();
            return ra-pa;
        }


        xReal Components:: score(XMul &xmul, const xReal K, const XReadable &C, const Level L) const
        {
            assert( running(C,L) );
            xmul.free();
            xmul << K;
            reac.activity(xmul,C,L);
            const xReal ra = xmul.product();

            assert(xmul.isEmpty());
            xmul << one;
            prod.activity(xmul,C,L);
            const xReal pa = xmul.product();
            const xReal Q_over_K = pa/ra;
            return Q_over_K.log();
        }


        xReal Components:: prodActivity(XMul &xmul, const XReadable &C, const Level L, const xReal xi) const
        {
            xmul.free();
            xmul << one;
            prod.activity(xmul,C,L,xi);
            return -xmul.product();
        }

        xReal Components:: reacActivity(XMul &xmul, const xReal K, const XReadable &C, const Level L, const xReal xi) const
        {
            xmul.free();
            xmul << K;
            reac.activity(xmul,C,L,-xi);
            return xmul.product();
        }

        xReal Components:: activity(XMul &xmul, const xReal K, const XReadable &C, const Level L, const xReal xi) const
        {
            return reacActivity(xmul, K, C, L, xi) + prodActivity(xmul, C, L, xi);
        }



        Situation Components:: situation(const XReadable &C, const Level L) const noexcept
        {
            if( reac.haveZero(C,L) )
            {
                if( prod.haveZero(C,L) )
                {
                    return Blocked;
                }
                else
                {
                    return Crucial;
                }
            }
            else
            {
                if( prod.haveZero(C,L) )
                {
                    return Crucial;
                }
                else
                {
                    return Running;
                }
            }
            
        }


        bool Components:: running(const XReadable &C, const Level L) const noexcept
        {
            return reac.positive(C,L) && prod.positive(C,L);
        }


        SignType Components:: bracket(XTriplet &       xi,
                                      XTriplet &       ff,
                                      XMul     &       xmul,
                                      const xReal      K,
                                      const XReadable &C,
                                      const Level      L) const
        {
            assert( Running == situation(C,L) || Crucial == situation(C,L) );
            const xReal zero;
            xi.a = xi.b = xi.c = zero;
            ff.a = ff.b = ff.c = activity(xmul, K, C, L);
            const SignType s = Sign::Of(ff.a);

            switch(s)
            {
                case __Zero__: break; // numeric zero
                case Positive: return positiveBracket(xi,ff,xmul,K,C,L);
                case Negative: return negativeBracket(xi,ff,xmul,K,C,L);
            }

            return s;
        }

        void Components:: moveSafely(XWritable &C, const Level L, const xReal xi) const noexcept
        {
            Coerce(prod).moveSafely(C,L,xi);
            Coerce(reac).moveSafely(C,L,-xi);
        }



        static const char NoNebulous[] = "forbidden Nebulous for bracketing";

        SignType Components:: positiveBracket(XTriplet &       xi,
                                              XTriplet &       ff,
                                              XMul    &        xmul,
                                              const xReal      K,
                                              const XReadable &C,
                                              const Level      L) const
        {
            assert(ff.a>0.0);
            //------------------------------------------------------------------
            // there is too much reactant/not enough product
            // forward xi>0 up to negative activity
            //------------------------------------------------------------------
            switch(attr)
            {
                case Nebulous:
                    throw Specific::Exception(name.c_str(),NoNebulous);

                case ProdOnly:
                    //----------------------------------------------------------
                    // use saturating expression
                    //----------------------------------------------------------
                    ++Coerce( (xi.c = K.pow( 1.0/d_nu )).exponent );
                    assert( prodActivity(xmul, C, L, xi.c) < 0.0 );
                    break;

                case ReacOnly:
                case Definite:
                    //----------------------------------------------------------
                    // use limiting reactant
                    //----------------------------------------------------------
                    xi.c = reac.limitingExtent(C,L);
                    assert( prodActivity(xmul, C, L, xi.c) < 0.0 ); // -1 if reac only
                    break;

            }

            ff.c = prodActivity(xmul, C, L, xi.c); assert(ff.c<0.0);
            return Positive;
        }


        SignType Components:: negativeBracket(XTriplet &       xi,
                                              XTriplet &       ff,
                                              XMul    &        xmul,
                                              const xReal      K,
                                              const XReadable &C,
                                              const Level      L) const
        {
            assert(ff.a<0.0);
            //------------------------------------------------------------------
            // there is too much product/not enough reactant
            // reverse xi<0 up to posivite activity
            //------------------------------------------------------------------
            switch(attr)
            {
                case Nebulous:
                    throw Specific::Exception(name.c_str(),NoNebulous);

                case ReacOnly:
                    //----------------------------------------------------------
                    // use saturation expression
                    //----------------------------------------------------------
                    ++Coerce( (xi.c = -K.pow( 1.0/d_nu )).exponent );
                    assert(reacActivity(xmul, K, C, L, xi.c) > 0.0);
                    break;

                case Definite:
                case ProdOnly:
                    //----------------------------------------------------------
                    // use limiting product
                    //----------------------------------------------------------
                    xi.c = -prod.limitingExtent(C,L);
                    assert(reacActivity(xmul, K, C, L, xi.c) > 0.0);
                    break;
            }

            ff.c = reacActivity(xmul, K, C, L, xi.c); assert(ff.c>0.0);
            return Negative;
        }


    }

}

