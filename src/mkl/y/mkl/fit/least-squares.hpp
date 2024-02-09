//! \file

#ifndef Y_Fit_Compute_Least_Squares_Included
#define Y_Fit_Compute_Least_Squares_Included 1

#include "y/mkl/fit/samples.hpp"
#include "y/mkl/fit/sequential.hpp"
#include "y/mkl/antelope/caddy.hpp"
#include "y/container/matrix.hpp"
#include "y/type/zeroed-field.hpp"
#include "y/oversized.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {

            //__________________________________________________________________
            //
            //
            //
            //! computing least squares
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class LeastSquares : public Oversized
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef          Sample<ABSCISSA,ORDINATE>     SampleType;     //!< alias
                typedef          TypeFor<ABSCISSA,ORDINATE>    MyType;         //!< alias
                typedef typename SampleType::Abscissae         Abscissae;      //!< alias
                typedef typename SampleType::Ordinates         Ordinates;      //!< alias
                typedef typename MyType::OutOfOrderFunc        OutOfOrderFunc; //!< alias
                typedef typename MyType::OutOfOrderGradient    OutOfOrderGrad; //!< alias
                typedef          Sequential<ABSCISSA,ORDINATE> SequentialFunc; //!< alias
                typedef          ListOf<LeastSquares>          List;           //!< alias
                typedef          Antelope::Add<ABSCISSA>       XAdd;           //!< alias
                typedef          Antelope::Caddy<ABSCISSA>     Caddy;          //!< alias
                typedef          typename Caddy::XNode         XNode;          //!< alias
                static const     size_t Dimension = MyType::Dimension;         //!< alias



                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
              
                //! initialize
                explicit LeastSquares() :
                xadd(),
                xlst(),
                dFda(),
                beta(),
                curv(),
                npts(0),
                zero(0),
                half(0.5),
                one(1),
                z___(),
                zord(*z___),
                next(0),
                prev(0)
                {

                }

                //! cleanup
                virtual ~LeastSquares() noexcept {}


                //______________________________________________________________
                //
                //
                // Methods for a single Sample
                //
                //______________________________________________________________
#include "least-squares/seq.hpp"
#include "least-squares/ooo.hpp"

              
                //______________________________________________________________
                //
                //
                // Methods for multiple samples
                //
                //______________________________________________________________
#include "least-squares/mix.hpp"

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                XAdd                          xadd; //!< to perform additions
                Caddy                         xlst; //!< to perform additions
                Vector<ORDINATE,SampleMemory> dFda; //!< local dF/da
                Vector<ABSCISSA,SampleMemory> beta; //!< gradient of D2
                Matrix<ABSCISSA>              curv; //!< approx curvature of D2
                const size_t                  npts; //!< last dimensions
                const ABSCISSA                zero; //!< alias
                const ABSCISSA                half; //!< alias
                const ABSCISSA                one;  //!< alias

            private:
                const ZeroedField<ORDINATE>   z___;
                const ORDINATE               &zord;

                Y_DISABLE_COPY_AND_ASSIGN(LeastSquares);

                static inline ABSCISSA FabsOf(const ABSCISSA x)
                {
                    return Fabs<ABSCISSA>::Of(x);
                }

                //! push |Bj-Fj|^2 into xadd
                inline void pushDSQ(const ORDINATE &Bj, const ORDINATE &Fj)
                {
                    const ORDINATE  dB = Bj - Fj;
                    const ABSCISSA *a  = SampleType::O2A(dB);
                    for(size_t d=0;d<Dimension;++d)
                        xadd << Squared(a[d]);
                }

                //! push |Bj-Fj|^2 into xadd
                /**

                 */
                inline void pushAll(const ORDINATE     &Bj,
                                    const ORDINATE     &Fj,
                                    const Booleans     &used)
                {

                    const ORDINATE  dB = Bj - Fj;
                    const ABSCISSA *df = SampleType::O2A(dB);
                    for(unsigned d=0;d<Dimension;++d)
                        xadd << Squared(df[d]);


                    // Outer loop over variables
                    const size_t nvars = beta.size();
                    XNode       *xnode = xlst.tail;
                    for(size_t i=nvars;i>0;--i,xnode=xnode->prev)
                    {
                        assert(xnode->indx == i);
                        if(!used[i])
                        {
                            assert( FabsOf(beta[i])<=zero) ;
                            continue;
                        }

                        const ABSCISSA *lhs = SampleType::O2A(dFda[i]);
                        for(unsigned d=0;d<Dimension;++d)
                        {
                            const ABSCISSA prod = df[d] * lhs[d];
                            //beta[i] += prod;
                            (*xnode) << prod;
                        }

                        for(size_t j=i;j>0;--j)
                        {
                            if(!used[j]) 
                            {
                                assert( FabsOf( curv[i][j]) <= zero );
                                continue;
                            }
                            const ABSCISSA *rhs = SampleType::O2A(dFda[j]);
                            for(unsigned d=0;d<Dimension;++d)
                            {
                                curv[i][j] += lhs[d] * rhs[d];
                            }
                        }
                    }

                }

            public:
                LeastSquares *next; //!< for list
                LeastSquares *prev; //!< for list
            };

        }

    }

}

#endif

