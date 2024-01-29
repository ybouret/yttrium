//! \file

#ifndef Y_Fit_Compute_Least_Squares_Included
#define Y_Fit_Compute_Least_Squares_Included 1

#include "y/mkl/fit/sample/interface.hpp"
#include "y/mkl/fit/sequential.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/container/matrix.hpp"
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
            //! computing least squares metric
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class ComputeLeastSquares
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Sample<ABSCISSA,ORDINATE>            SampleType;
                typedef TypeFor<ABSCISSA,ORDINATE>           MyType;

                typedef typename SampleType::Abscissae       Abscissae;
                typedef typename SampleType::Ordinates       Ordinates;
                typedef typename MyType::OutOfOrderFunc      OutOfOrderFunc;
                typedef typename MyType::OutOfOrderGradient  OutOfOrderGrad;
                typedef Sequential<ABSCISSA,ORDINATE>        SequentialFunc;
                static const size_t Dimension = MyType::Dimension;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit ComputeLeastSquares() :
                xadd(),
                dFda(),
                beta(),
                zero(0),
                half(0.5),
                one(1),
                z___(),
                zord(*z___)
                {

                }

                virtual ~ComputeLeastSquares() noexcept
                {

                }

                //______________________________________________________________
                //
                //
                // Methods for a single Sample
                //
                //______________________________________________________________

                //! compute least squares of S w.r.t a sequential F
                inline ABSCISSA Of(SequentialFunc           &F,
                                   const SampleType         &S,
                                   const Readable<ABSCISSA> &aorg,
                                   const Variables          &vars)
                {
                    //----------------------------------------------------------
                    // initialize
                    //----------------------------------------------------------
                    const size_t     n = S.numPoints();
                    const Abscissae &a = S.abscissae();
                    const Ordinates &b = S.ordinates();

                    xadd.make(n*Dimension);

                    //----------------------------------------------------------
                    // first point
                    //----------------------------------------------------------
                    {
                        const size_t   j  = S.indx[1];
                        const ORDINATE Fj = F.set(a[j],aorg,vars);
                        push(b[j],Fj);
                    }

                    //----------------------------------------------------------
                    // following points
                    //----------------------------------------------------------
                    for(size_t i=2;i<=n;++i)
                    {
                        const size_t   j  = S.indx[i];
                        const ORDINATE Fj = F.run(a[j],aorg,vars);
                        push(b[j],Fj);
                    }

                    //----------------------------------------------------------
                    // return sum
                    //----------------------------------------------------------
                    return half * xadd.sum();
                }

                //! compute least squares of S w.r.t an out of order F
                inline ABSCISSA Of(OutOfOrderFunc           &F,
                                   const SampleType         &S,
                                   const Readable<ABSCISSA> &aorg,
                                   const Variables          &vars)
                {
                    //----------------------------------------------------------
                    // initialize
                    //----------------------------------------------------------
                    const size_t     n = S.numPoints();
                    const Abscissae &a = S.abscissae();
                    const Ordinates &b = S.ordinates();
                    xadd.make(n*Dimension);

                    //----------------------------------------------------------
                    // compute
                    //----------------------------------------------------------
                    for(size_t j=n;j>0;--j)
                    {
                        const ORDINATE Fj = F(a[j],aorg,vars);
                        push(b[j],Fj);
                    }

                    //----------------------------------------------------------
                    // return sum
                    //----------------------------------------------------------
                    return half * xadd.sum();
                }




                inline ABSCISSA Of(OutOfOrderFunc           &F,
                                   const SampleType         &S,
                                   const Readable<ABSCISSA> &aorg,
                                   const Variables          &vars,
                                   const Booleans           &used,
                                   OutOfOrderGrad           &G,
                                   Matrix<ABSCISSA>         &alpha)

                {
                    // initialize
                    const size_t     n  = S.numPoints();
                    const Abscissae &a  = S.abscissae();
                    const Ordinates &b  = S.ordinates();
                    const size_t     nv = aorg.size();
                    assert(used.size() == nv);
                    assert(alpha.cols  == nv);
                    assert(alpha.rows  == nv);

                    xadd.make(n*Dimension);
                    dFda.adjust(nv,zord);
                    beta.adjust(nv,zero);

                    // global setting
                    beta.ld(zero);
                    alpha.ld(zero);

                    for(size_t j=n;j>0;--j)
                    {
                        const ORDINATE Fj = F(a[j],aorg,vars);
                        dFda.ld(zord); // local setting befor call
                        G(dFda,a[j],aorg,vars,used);
                        push(alpha,beta,b[j],Fj);
                    }

                    // ensure consistency
                    for(size_t i=nv;i>0;--i)
                    {
                        if( vars.found(i) && used[i] )
                        {
                            for(size_t j=i-1;j>0;--j)
                                alpha[j][i] = alpha[i][j];
                            continue;
                        }

                        // assuming not used by algorithm
                        beta[i] = zero;
                        alpha.ldCol(i,zero);
                        alpha.ldRow(i,zero);
                        alpha[i][i] = one;
                    }

                    return half * xadd.sum();
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Antelope::Add<ABSCISSA>       xadd; //!< to perform additions
                Vector<ORDINATE,SampleMemory> dFda; //!< local dF/da
                Vector<ABSCISSA,SampleMemory> beta; //!< gradient of D2
                const ABSCISSA                zero; //!< alias
                const ABSCISSA                half; //!< alias
                const ABSCISSA                one;  //!< alias

            private:
                const ZeroedField<ORDINATE>   z___;
                const ORDINATE               &zord;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ComputeLeastSquares);

                inline void push(const ORDINATE &Bj, const ORDINATE &Fj)
                {
                    const ORDINATE  dB = Bj - Fj;
                    const ABSCISSA *a  = SampleType::O2A(dB);
                    for(size_t d=0;d<Dimension;++d)
                        xadd << Squared(a[d]);
                }

                inline void push(Matrix<ABSCISSA>   &alpha,
                                 Writable<ABSCISSA> &beta,
                                 const ORDINATE     &Bj,
                                 const ORDINATE     &Fj)
                {
                    assert(beta.size()==dFda.size());
                    const ORDINATE  dB = Bj - Fj;
                    const ABSCISSA *df = SampleType::O2A(dB);
                    for(unsigned d=0;d<Dimension;++d)
                        xadd << Squared(df[d]);

                    const size_t nv = beta.size();
                    for(size_t i=nv;i>0;--i)
                    {
                        const ABSCISSA *lhs = SampleType::O2A(dFda[i]);
                        for(unsigned d=0;d<Dimension;++d)
                        {
                            beta[i] += df[d] * lhs[d];
                        }
                        for(size_t j=i;j>0;--j)
                        {
                            const ABSCISSA *rhs = SampleType::O2A(dFda[j]);
                            for(unsigned d=0;d<Dimension;++d)
                            {
                                alpha[i][j] += lhs[d] * rhs[d];
                            }
                        }
                    }
                }
            };

        }

    }

}

#endif

