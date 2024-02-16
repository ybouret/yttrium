//! \file

#ifndef Y_Fit_Least_Squares_Included
#define Y_Fit_Least_Squares_Included 1

#include "y/mkl/fit/least-squares/com.hpp"
#include "y/type/zeroed-field.hpp"

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
            class LeastSquares : public LeastSquaresCom<ABSCISSA>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef          Sample<ABSCISSA,ORDINATE>     SampleType;     //!< alias
                typedef          Samples<ABSCISSA,ORDINATE>    SamplesType;    //!< alias
                typedef          TypeFor<ABSCISSA,ORDINATE>    MyType;         //!< alias
                typedef typename SampleType::Abscissae         Abscissae;      //!< alias
                typedef typename SampleType::Ordinates         Ordinates;      //!< alias
                typedef typename SampleType::Predicted         Predicted;      //!< alias
                typedef typename MyType::OutOfOrderFunc        OutOfOrderFunc; //!< alias
                typedef typename MyType::OutOfOrderGradient    OutOfOrderGrad; //!< alias
                typedef          Sequential<ABSCISSA,ORDINATE> SequentialFunc; //!< alias
                typedef          CxxListOf<LeastSquares>       List;           //!< alias
                typedef          LeastSquaresCom<ABSCISSA>     Common;         //!< alias
                typedef          typename Common::XNode        XNode;          //!< alias
                static const     size_t Dimension = MyType::Dimension;         //!< alias

                using Common::xadd;
                using Common::xlst;
                using Common::npts;
                using Common::curv;
                using Common::zero;
                using Common::beta;
                using Common::half;
                using Common::one;
                using Common::aabs;
                
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
              
                //! initialize
                explicit LeastSquares() :
                Common(),
                dFda(),
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
                Vector<ORDINATE,MemoryModel>  dFda; //!< local dF/da

            private:
                const ZeroedField<ORDINATE>   z___;
                const ORDINATE               &zord;

                Y_DISABLE_COPY_AND_ASSIGN(LeastSquares);



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
                            assert( aabs(beta[i])<=zero) ;
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
                                assert( aabs( curv[i][j]) <= zero );
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

