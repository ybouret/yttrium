
#include "y/chemical/reactive/equilibrium/aftermath.hpp"
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
            class Gauge
            {
            public:
                inline Gauge(const Components & _E,
                const xReal _K0,
                XWritable  &_C0,
                const Level _L0,
                XMul &_X) :
                E(_E),
                K(_K0),
                C(_C0),
                L(_L0),
                X(_X),
                zero(0.0),
                half(0.5)
                {
                }

                inline ~Gauge() noexcept {}

                const Components &E;
                const xReal       K;
                XWritable        &C;
                const Level       L;
                XMul             &X;
                const xReal       zero;
                const xReal       half;

                SignType moved(xReal &xi)
                {
                    const xReal  zero;
                    XTriplet     x = { zero, zero, zero };
                    XTriplet     f = { zero, zero, zero };
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
                        const SignType s = E.bracket(x,f,X,K,C,L);
                        switch(s)
                        {
                            case __Zero__: xi = zero; return  __Zero__; // no need to move

                            case Positive:
                                x_pos = &x.a; f_pos = &f.a; assert(f.a>0.0);
                                x_neg = &x.c; f_neg = &f.c; assert(f.c<0.0);
                                break;

                            case Negative:
                                x_neg = &x.a; f_neg = &f.a; assert(f.a<0.0);
                                x_pos = &x.c; f_pos = &f.c; assert(f.c>0.0);
                                break;
                        }
                    }



                    //----------------------------------------------------------
                    //
                    // maximal bisection
                    //
                    //----------------------------------------------------------
                    const xReal  half  = 0.5;
                    xReal        width = (*x_pos - *x_neg).abs();
                    for(;;)
                    {
                        x.b = half * (*x_pos + *x_neg);
                        f.b = E.activity(X,K,C,L,x.b);
                        const SignType s = Sign::Of(f.b);

                        switch(s)
                        {
                            case __Zero__:
                                E.moveSafely(C, L,xi=x.b);
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
                            E.moveSafely(C,L,xi=x.b);
                            return s;
                        }
                        width = newWidth;
                    }

                }

                Y_DISABLE_COPY_AND_ASSIGN(Gauge);

            };




        }

        // static inline real_t toReal(const xReal &x) { return real_t(x); }

        Outcome Aftermath:: solve(const Components & eq,
                                  const xReal         eK,
                                  XWritable         & C1,
                                  const Level         L1,
                                  XWritable  &        C0,
                                  const Level         L0)
        {

            //------------------------------------------------------------------
            //
            // initialize status
            //
            //------------------------------------------------------------------
            const Situation st = eq.situation(C0,L0);
            switch(st)
            {
                case Blocked: return Outcome(Blocked,eq,eK,C0,L0); // early return
                case Running:
                case Crucial:
                    break;
            }

            //------------------------------------------------------------------
            //
            // prepare outcome
            //
            //------------------------------------------------------------------
            const Outcome outcome(st,eq,eK,C1,L1);

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
            Gauge F(eq,eK,C1,L1,xmul);

            //------------------------------------------------------------------
            //
            // initialize first xi
            //
            //------------------------------------------------------------------
            xReal    x = zero;
            SignType s = F.moved(x);
            if(__Zero__==s) goto DONE; // already solved

            //------------------------------------------------------------------
            //
            // look for next xi
            //
            //------------------------------------------------------------------
            for(;;)
            {
                xReal x_new = zero;
                s = F.moved(x_new);

                if(__Zero__==s)          goto DONE; //!< numerical zero
                if(x_new.abs()>=x.abs()) goto DONE; //!< numerical zero

                x = x_new;
            }

        DONE:
            // done
            return outcome;
        }


        xReal  Aftermath:: extent(const Outcome &outcome, const XReadable &C0, const Level L0)
        {
            return outcome.extent(xadd,C0,L0);
        }

        xReal  Aftermath:: extent(const Outcome &outcome, const XReadable &C0, const Level L0, XWritable &dC, const Level dL)
        {
            return outcome.extent(xadd,C0,L0,dC,dL);
        }

    }

}
