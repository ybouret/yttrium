//! \file

#ifndef Y_MKL_Filter_Selector_Included
#define Y_MKL_Filter_Selector_Included 1


#include "y/sequence/vector.hpp"
#include "y/mkl/api.hpp"

namespace Yttrium
{
    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //! selecting point within a window
        //
        //______________________________________________________________________
        struct Selector
        {


            //__________________________________________________________________
            //
            //! select
            /**
             \param x     value of interest
             \param left  minimal: x - |left|
             \param right maximal: x + |right|
             \param xs    output selected x
             \param ys    output selected y
             \param X     increasing values of X
             \param Y     Y(X)
             */
            //__________________________________________________________________
            template <typename T,
            typename ALLOCATOR,
            typename XSOURCE,
            typename YSOURCE> static inline
            void Fill(const T               x,
                      T                     left,
                      T                     right,
                      Vector<T,ALLOCATOR> & xs,
                      Vector<T,ALLOCATOR> & ys,
                      XSOURCE &             X,
                      YSOURCE &             Y)
            {
                //______________________________________________________________
                //
                // setup
                //______________________________________________________________
                assert(X.size()==Y.size());
                const size_t n = X.size();
                left  = Fabs<T>::Of(left);
                right = Fabs<T>::Of(right);
                xs.free();
                ys.free();
                switch(n)
                {
                    case 0:
                        return;
                    case 1:
                        xs << X[1];
                        ys << Y[1];
                        return;
                    default:
                        break;
                }
                assert(n>=2);

                //______________________________________________________________
                //
                // locate lower index
                //______________________________________________________________
                size_t lower = 1;
                if(x<=X[lower])
                {
                    goto SELECT;
                }
                else
                {
                    if(X[n]<=x)
                    {
                        lower = n;
                        goto SELECT;
                    }
                    else
                    {
                        size_t upper = n;
                        while(upper-lower>1)
                        {
                            const size_t probe = (lower+upper)>>1;
                            switch( Sign::Of(x,X[probe]))
                            {
                                case __Zero__: lower = probe; goto SELECT;
                                case Negative: upper = probe; continue;
                                case Positive: lower = probe; continue;
                            }
                        }

                    }
                }
            SELECT:
                //______________________________________________________________
                //
                // estimate boundaries
                //______________________________________________________________
                size_t upper = lower;
                {
                    const  T xmax = x + right;
                    while(upper<n && X[upper] <= xmax) ++upper;
                }
                {
                    const T xmin = x - left;
                    while(lower>1 && X[lower] >= xmin) --lower;
                }
                std::cerr << lower << " -> " << upper << std::endl;

                //______________________________________________________________
                //
                // always take a second point
                //______________________________________________________________
                if(lower==upper)
                {
                    if(1==lower)
                    {
                        upper=2;
                    }
                    else
                    {
                        if(n==lower)
                        {
                            upper=n;
                            --lower;
                        }
                        else
                        {
                            assert(lower>1);
                            assert(lower<n);
                            const size_t iprev = lower-1;
                            const size_t inext = lower+1;
                            const T      dprev = Fabs<T>::Of(X[iprev]-X[lower]);
                            const T      dnext = Fabs<T>::Of(X[inext]-X[lower]);
                            if(dprev<dnext)
                            {
                                upper = lower;
                                lower = iprev;
                            }
                            else
                            {
                                upper = inext;
                            }
                        }
                    }
                }

                //______________________________________________________________
                //
                // fill
                //______________________________________________________________
                for(size_t i=lower;i<=upper;++i)
                {
                    xs << X[i];
                    ys << Y[i];
                }

            }


        };

    }
}


#endif
