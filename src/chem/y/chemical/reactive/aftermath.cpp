
#include "y/chemical/reactive/aftermath.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/root/zbis.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Aftermath:: Aftermath() : xmul(), xadd(), zero()
        {
        }

        Aftermath:: ~Aftermath() noexcept
        {
        }


        namespace
        {
            struct EqIndicator
            {
                const Components &eq;
                const xReal       K;
                XWritable        &C;
                const Level       L;
                XMul             &xmul;

                xReal operator()(const xReal xi)
                {
                    return eq.activity(xmul, K, C, L, xi);
                }

                SignType moved(xReal &xi)
                {
                    const xReal  half = 0.5;
                    XTriplet     x;
                    XTriplet     f;
                    xReal *      f_pos = 0;
                    xReal *      x_pos = 0;
                    xReal *      f_neg = 0;
                    xReal *      x_neg = 0;

                    //----------------------------------------------------------
                    //
                    // initialize by bracketing
                    //
                    //----------------------------------------------------------
                    {
                        const SignType s = eq.bracket(x,f,xmul,K,C,L);
                        switch(s)
                        {
                            case __Zero__:
                                xi = 0;
                                std::cerr << "not moved :)" << std::endl;
                                return  __Zero__; // no need to move

                            case Positive:
                                x_pos = &x.a; f_pos = &f.a; assert(f.a>0.0);
                                x_neg = &x.c; f_neg = &f.c; assert(f.c<0.0);
                                break;

                            case Negative:
                                x_pos = &x.c; f_pos = &f.c; assert(f.c>0.0);
                                x_neg = &x.a; f_neg = &f.a; assert(f.a<0.0);
                                break;
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // maximal bisection
                    //
                    //----------------------------------------------------------
                    xReal width = (*x_pos - *x_neg).abs();
                    for(;;)
                    {
                        x.b = half * (*x_pos + *x_neg);
                        f.b = eq.activity(xmul,K,C,L,x.b);
                        const SignType s = Sign::Of(f.b);
                        switch(s)
                        {
                            case __Zero__:
                                // early return
                                eq.moveSafely(C, L,xi=x.b);
                                return __Zero__;

                            case Positive:
                                *x_pos = x.b;
                                *f_pos = f.b;
                                break;

                            case Negative:
                                *x_neg = x.b;
                                *f_neg = f.b;
                                break;
                        }

                        const xReal newWidth = (*x_pos - *x_neg).abs();
                        if(newWidth>=width)
                        {
                            eq.moveSafely(C,L,xi=x.b);
                            return s;
                        }
                        width = newWidth;
                    }

                }

            };




        }

        // static inline real_t toReal(const xReal &x) { return real_t(x); }

        Outcome Aftermath:: solve(const Components &eq,
                                  const xReal       eK,
                                  XWritable        &C1,
                                  const Level       L1,
                                  const XReadable  &C0,
                                  const Level       L0)
        {

            //------------------------------------------------------------------
            //
            // initialize status
            //
            //------------------------------------------------------------------
            const Situation st = eq.situation(C0,L0);
            switch(st)
            {
                case Blocked: return Outcome(Blocked,eq,eK,C0,L0);
                case Running:
                    break;
            }

            //------------------------------------------------------------------
            //
            // prepare outcome
            //
            //------------------------------------------------------------------
            const Outcome outcome(Running,eq,eK,C1,L1);
            std::cerr << "Running" << std::endl;

            //------------------------------------------------------------------
            //
            // prepare workspace with initial conditions
            //
            //------------------------------------------------------------------
            eq.transfer(C1,L1,C0,L0);

            //------------------------------------------------------------------
            //
            // prepare equilibrium indicator
            //
            //------------------------------------------------------------------
            EqIndicator F = { eq, eK, C1, L1 , xmul};

            //------------------------------------------------------------------
            //
            // initialize first xi
            //
            //------------------------------------------------------------------
            xReal    x = zero;
            SignType s = F.moved(x);
            std::cerr << "xi0 = " << real_t(x) << " => " << real_t(eq.activity(xmul, eK, C1, L1)) <<  " @" << C1 << std::endl;
            if(__Zero__==s)
                goto DONE; // already solved

            //------------------------------------------------------------------
            //
            // look for next xi
            //
            //------------------------------------------------------------------
            for(;;)
            {
                xReal x_new = zero;
                s = F.moved(x_new);
                std::cerr << "xi  = " << real_t(x_new) << " => " << real_t(eq.activity(xmul, eK, C1, L1)) <<  " @" << C1 << std::endl;

                if(__Zero__==s)
                    goto DONE;

                if(x_new.abs()>=x.abs()) {
                    goto DONE;
                }

                x = x_new;
            }

        DONE:
            // done
            return outcome;
        }
    }

}
