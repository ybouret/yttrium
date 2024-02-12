//! \file

#ifndef Y_Fit_Step_Inventor_Included
#define Y_Fit_Step_Inventor_Included 1

#include "y/mkl/fit/least-squares.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/calculus/ipower.hpp"
#include "y/mkl/numeric.hpp"

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
            //! Findind Out step according to a full metrics
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA>
            class StepInventor : public Oversized
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                inline explicit StepInventor() :
                curv(),
                step(),
                atry(),
                lu(),
                pmin( -int(Numeric<ABSCISSA>::DIG)-1   ),
                pmax( Numeric<ABSCISSA>::MAX_10_EXP-1  ),
                zero(0),
                one(1),
                two(2),
                ten(10),
                tenth(0.1)
                {}

                //! cleanup
                inline virtual ~StepInventor() noexcept {}

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! prepare memory
                inline void prepare(const size_t nvar)
                {
                    curv.make(nvar,nvar);
                    step.adjust(nvar,zero);
                    atry.adjust(nvar,zero);
                    lu.ensure(nvar);
                }

                //! compute factor
                ABSCISSA getFactor(const int param)
                {
                    assert(param>=pmin);
                    assert(param<=pmax);
                    if(param<=pmin) return one;

                    switch( Sign::Of(param) )
                    {
                        case __Zero__: break;
                        case Positive: return one + ipower(ten,param);
                        case Negative: return one + ipower(tenth,-param);
                    }
                    return two;
                }

                //! compute step
                template <typename LEAST_SQUARES>
                bool compute(const LEAST_SQUARES &ls, 
                             const int            p,
                             const Booleans      &used)
                {
                    const Readable<ABSCISSA> &beta  = ls.beta;
                    const Matrix<ABSCISSA>   &alpha = ls.curv;
                    const size_t              nvar  = beta.size();
                    assert(nvar>0);

                    prepare(nvar);

                    const ABSCISSA fac = getFactor(p);


                    for(size_t i=nvar;i>0;--i)
                    {
                        step[i] = beta[i];
                        for(size_t j=nvar;j>0;--j)
                        {
                            curv[i][j] = alpha[i][j];
                        }
                        if(used[i])
                            curv[i][i] *= fac;
                    }

                    std::cerr << "Curv = " << curv << std::endl;
                    if(!lu.build(curv))
                    {
                        std::cerr << "Singular Matrix" << std::endl;
                        return false;
                    }

                    lu.solve(curv,step);
                    std::cerr << "step = " << step << std::endl;


                    return false;

                }

            private:
                bool buildCurvature(const Matrix<ABSCISSA> &alpha,
                                    const int               param,
                                    const Booleans         &used)
                {
                    const ABSCISSA fac = getFactor(param);
                    curv.assign(alpha);
                    for(size_t i=curv.rows;i>0;--i) 
                    {
                        if(used[i])
                            curv[i][i] *= fac;
                    }
                    std::cerr << "Curv=" << curv << std::endl;
                    return lu.build(curv);
                }

            public:
                //! compute step
                template <typename LEAST_SQUARES>
                bool buildStep(const LEAST_SQUARES &ls,
                               int                 &p,
                               const Booleans      &used,
                               bool                &kept)
                {
                    assert(p>=pmin);
                    assert(p<=pmax);

                    const Readable<ABSCISSA> &beta  = ls.beta;
                    const Matrix<ABSCISSA>   &alpha = ls.curv;
                    const size_t              nvar  = beta.size();
                    prepare(nvar);

                    //----------------------------------------------------------
                    // look for invertible matrix
                    //----------------------------------------------------------
                    kept = true;
                    while( !buildCurvature(alpha,p,used) )
                    {
                        kept = false; // must reduce step
                        if(++p>pmax)
                        {
                            p = pmax;
                            return false;
                        }
                    }
                    
                    //----------------------------------------------------------
                    // compute local step
                    //----------------------------------------------------------
                    for(size_t i=nvar;i>0;--i) step[i] = beta[i];
                    std::cerr << "Beta=" << beta << std::endl;
                    lu.solve(curv,step);
                    return true;
                }



                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Matrix<ABSCISSA>              curv; //!< approx curvature
                Vector<ABSCISSA,SampleMemory> step; //!< approx step
                Vector<ABSCISSA,SampleMemory> atry; //!< trial parameters
                LU<ABSCISSA>                  lu;   //!< linear algebra
                const int                     pmin; //!< -int(DIG)-1  => lambda=1
                const int                     pmax; //!< MAX_10_EXP   => overflow

            private:
                const ABSCISSA                zero;
                const ABSCISSA                one;
                const ABSCISSA                two;
                const ABSCISSA                ten;
                const ABSCISSA                tenth;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(StepInventor);
            };

        }

    }

}

#endif
