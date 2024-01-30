//! \file

#ifndef Y_Fit_Compute_Least_Squares_Included
#define Y_Fit_Compute_Least_Squares_Included 1

#include "y/mkl/fit/samples.hpp"
#include "y/mkl/fit/sequential.hpp"
#include "y/mkl/antelope/add.hpp"
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
            //! computing least squares metric
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class ComputeLeastSquares : public Oversized
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

                typedef ListOf<ComputeLeastSquares> List;
                
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
                npts(0),
                last(0),
                zero(0),
                half(0.5),
                one(1),
                z___(),
                zord(*z___),
                next(0),
                prev(0)
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
                        pushDSQ(b[j],Fj);
                    }

                    //----------------------------------------------------------
                    // following points
                    //----------------------------------------------------------
                    for(size_t i=2;i<=n;++i)
                    {
                        const size_t   j  = S.indx[i];
                        const ORDINATE Fj = F.run(a[j],aorg,vars);
                        pushDSQ(b[j],Fj);
                    }

                    //----------------------------------------------------------
                    // return sum
                    //----------------------------------------------------------
                    Coerce(npts) = n;
                    return ( Coerce(last) = half * xadd.sum() );
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
                        pushDSQ(b[j],Fj);
                    }

                    //----------------------------------------------------------
                    // return sum
                    //----------------------------------------------------------
                    Coerce(npts) = n;
                    return ( Coerce(last) = half * xadd.sum() );
                }




                inline ABSCISSA Of(OutOfOrderFunc           &F,
                                   const SampleType         &S,
                                   const Readable<ABSCISSA> &aorg,
                                   const Variables          &vars,
                                   const Booleans           &used,
                                   OutOfOrderGrad           &G)

                {
                    // initialize
                    const size_t     n  = S.numPoints();
                    const Abscissae &a  = S.abscissae();
                    const Ordinates &b  = S.ordinates();
                    const size_t     nv = aorg.size();
                    assert(used.size() == nv);

                    xadd.make(n*Dimension);
                    dFda.adjust(nv,zord);
                    beta.adjust(nv,zero);
                    curv.make(nv,nv);

                    // global setting
                    beta.ld(zero);
                    curv.ld(zero);

                    for(size_t j=n;j>0;--j)
                    {
                        const ORDINATE Fj = F(a[j],aorg,vars);
                        dFda.ld(zord); // local setting befor call
                        G(dFda,a[j],aorg,vars,used);
                        pushAll(b[j],Fj);
                    }

                    // ensure consistency
                    for(size_t i=nv;i>0;--i)
                    {
                        if( vars.found(i) && used[i] )
                        {
                            for(size_t j=i-1;j>0;--j)
                                curv[j][i] = curv[i][j];
                            continue;
                        }

                        // assuming not used by algorithm
                        beta[i] = zero;
                        curv.ldCol(i,zero);
                        curv.ldRow(i,zero);
                        curv[i][i] = one;
                    }

                    Coerce(npts) = 0;
                    return ( Coerce(last) = half * xadd.sum() );
                }

                inline ABSCISSA Of(const List &L)
                {

                    if(L.size<=0) return zero;

                    // prepare memory
                    {
                        const ComputeLeastSquares &first = *(L.head);
                        const size_t nv = first.dFda.size(); assert(nv>0);
                        dFda.adjust(nv,zero);
                        beta.adjust(nv,zero);
                        curv.make(nv,nv);
                        xadd.make(L.size);
                    }

                    // first pass: npts
                    Coerce(npts) = 0;
                    for(const ComputeLeastSquares *cls = L.head; cls; cls=cls->next)
                    {
                        Coerce(npts) += cls->npts;
                    }

                    // second pass: beta
                    const size_t   nvar = beta.size();
                    const ABSCISSA den  = ABSCISSA(npts);
                    for(size_t i=nvar;i>0;--i)
                    {
                        xadd.free();
                        for(const ComputeLeastSquares *cls = L.head; cls; cls=cls->next)
                        {
                            xadd << (cls->beta[i]* static_cast<ABSCISSA>(cls->npts) );
                        }
                        beta[i] = xadd.sum() / den;
                    }

                    // third pass: curv
                    for(size_t i=nvar;i>0;--i)
                    {
                        for(size_t j=i;j>0;--j)
                        {
                            xadd.free();
                            for(const ComputeLeastSquares *cls = L.head; cls; cls=cls->next)
                            {
                                xadd << (cls->curv[i][j]*static_cast<ABSCISSA>(cls->npts));
                            }
                            curv[i][j] = curv[j][i] = xadd.sum() / den;
                        }
                    }

                    // final D2
                    xadd.free();
                    for(const ComputeLeastSquares *cls = L.head; cls; cls=cls->next)
                    {
                        xadd << (cls->last * static_cast<ABSCISSA>(cls->npts));
                    }
                    return ( Coerce(last) = xadd.sum()/den );

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
                Matrix<ABSCISSA>              curv; //!< approx curvature of D2
                const size_t                  npts; //!< last dimensions
                const ABSCISSA                last; //!< last valyue
                const ABSCISSA                zero; //!< alias
                const ABSCISSA                half; //!< alias
                const ABSCISSA                one;  //!< alias

            private:
                const ZeroedField<ORDINATE>   z___;
                const ORDINATE               &zord;

                Y_DISABLE_COPY_AND_ASSIGN(ComputeLeastSquares);

                inline void pushDSQ(const ORDINATE &Bj, const ORDINATE &Fj)
                {
                    const ORDINATE  dB = Bj - Fj;
                    const ABSCISSA *a  = SampleType::O2A(dB);
                    for(size_t d=0;d<Dimension;++d)
                        xadd << Squared(a[d]);
                }

                inline void pushAll(const ORDINATE     &Bj,
                                    const ORDINATE     &Fj)
                {
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
                                curv[i][j] += lhs[d] * rhs[d];
                            }
                        }
                    }
                }

            public:
                ComputeLeastSquares          *next;
                ComputeLeastSquares          *prev;
            };

        }

    }

}

#endif

