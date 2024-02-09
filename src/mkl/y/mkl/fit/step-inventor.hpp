//! \file

#ifndef Y_Fit_Compute_Step_Inventor_Included
#define Y_Fit_Compute_Step_Inventor_Included 1

#include "y/mkl/fit/least-squares.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/calculus/ipower.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {

            template <typename ABSCISSA>
            class StepInventor
            {
            public:

                inline explicit StepInventor() :
                curv(),
                step(),
                lu(),
                zero(0),
                one(1),
                two(2),
                ten(10),
                tenth(0.1)
                {}

                inline virtual ~StepInventor() noexcept {}

                inline void prepare(const size_t nvar)
                {
                    curv.make(nvar,nvar);
                    step.adjust(nvar,zero);
                    lu.ensure(nvar);
                }

                ABSCISSA getFactor(const int p)
                {

                    switch( Sign::Of(p) )
                    {
                        case __Zero__: break;
                        case Positive: return one + ipower(ten,p);
                        case Negative: return one + ipower(tenth,-p);
                    }
                    return two;
                }

                template <typename LEAST_SQUARES>
                bool compute(const LEAST_SQUARES &ls, 
                             const int            p,
                             const Booleans      &used)
                {
                    const Readable<ABSCISSA> &beta  = ls.beta;
                    const Matrix<ABSCISSA>   &alpha = ls.curv;
                    const size_t              nvar  = beta.size();
                    
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

                Matrix<ABSCISSA>              curv;
                Vector<ABSCISSA,SampleMemory> step;
                LU<ABSCISSA>                  lu;
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
