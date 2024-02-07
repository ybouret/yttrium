//! \file

#ifndef Y_Fit_Compute_Least_Squares_Included
#define Y_Fit_Compute_Least_Squares_Included 1

#include "y/mkl/fit/samples.hpp"
#include "y/mkl/fit/sequential.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/container/matrix.hpp"
#include "y/type/zeroed-field.hpp"
#include "y/oversized.hpp"
#include "y/data/list/cxx.hpp"

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
                dFda(),
                beta(),
                curv(),
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

                //! cleanup
                virtual ~LeastSquares() noexcept {}


                

                //______________________________________________________________
                //
                //
                // Methods for a single Sample
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //! compute least squares of S w.r.t a sequential F
                /**
                 \param F    a sequential function
                 \param S    a single sample
                 \param aorg parameters
                 \param vars variables
                 */
                //______________________________________________________________
                inline ABSCISSA Of(SequentialFunc           &F,
                                   const SampleType         &S,
                                   const Readable<ABSCISSA> &aorg,
                                   const Variables          &vars)
                {
                    //----------------------------------------------------------
                    // initialize
                    //----------------------------------------------------------
                    const size_t     np = S.numPoints();
                    const Abscissae &a  = S.abscissae();
                    const Ordinates &b  = S.ordinates();
                    xadd.make(np*Dimension);

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
                    for(size_t i=2;i<=np;++i)
                    {
                        const size_t   j  = S.indx[i];
                        const ORDINATE Fj = F.run(a[j],aorg,vars);
                        pushDSQ(b[j],Fj);
                    }

                    //----------------------------------------------------------
                    // return sum
                    //----------------------------------------------------------
                    Coerce(npts) = np;
                    return ( Coerce(last) = half * xadd.sum() );
                }

                //______________________________________________________________
                //
                //! compute least squares of S w.r.t an out of order F
                /**
                 \param F    an out-of-order function
                 \param S    a single sample
                 \param aorg parameters
                 \param vars variables
                 */
                //______________________________________________________________
                inline ABSCISSA Of(OutOfOrderFunc           &F,
                                   const SampleType         &S,
                                   const Readable<ABSCISSA> &aorg,
                                   const Variables          &vars)
                {
                    //----------------------------------------------------------
                    // initialize
                    //----------------------------------------------------------
                    const size_t     np = S.numPoints();
                    const Abscissae &a  = S.abscissae();
                    const Ordinates &b  = S.ordinates();
                    xadd.make(np*Dimension);

                    //----------------------------------------------------------
                    // compute
                    //----------------------------------------------------------
                    for(size_t j=np;j>0;--j)
                    {
                        const ORDINATE Fj = F(a[j],aorg,vars);
                        pushDSQ(b[j],Fj);
                    }

                    //----------------------------------------------------------
                    // return sum
                    //----------------------------------------------------------
                    Coerce(npts) = np;
                    return ( Coerce(last) = half * xadd.sum() );
                }



                //______________________________________________________________
                //
                //! compute full least squares of S w.r.t an out-of-order F
                /**
                 \param F    an out of order function
                 \param S    a single sample
                 \param aorg parametes
                 \param vars variables
                 \param used used parameters/variables
                 \param G   an out of order gradient
                 */
                //______________________________________________________________
                inline ABSCISSA Of(OutOfOrderFunc           &F,
                                   const SampleType         &S,
                                   const Readable<ABSCISSA> &aorg,
                                   const Variables          &vars,
                                   const Booleans           &used,
                                   OutOfOrderGrad           &G)

                {
                    //----------------------------------------------------------
                    // initialize
                    //----------------------------------------------------------
                    const size_t     np = S.numPoints();
                    const Abscissae &a  = S.abscissae();
                    const Ordinates &b  = S.ordinates();
                    const size_t     nv = aorg.size();
                    assert(used.size() == nv);

                    xadd.make(np*Dimension);
                    dFda.adjust(nv,zord);
                    beta.adjust(nv,zero);
                    curv.make(nv,nv);

                    //----------------------------------------------------------
                    // global initialization
                    //----------------------------------------------------------
                    beta.ld(zero);
                    curv.ld(zero);

                    //----------------------------------------------------------
                    // accumulation
                    //----------------------------------------------------------
                    for(size_t j=np;j>0;--j)
                    {
                        const ORDINATE Fj = F(a[j],aorg,vars);
                        dFda.ld(zord); // local setting before call
                        G(dFda,a[j],aorg,vars,used);
                        pushAll(b[j],Fj,used);
                    }

                    //----------------------------------------------------------
                    // ensure consistency
                    //----------------------------------------------------------
                    for(size_t i=nv;i>0;--i)
                    {
                        if( vars.found(i) && used[i] )
                        {
                            for(size_t j=i-1;j>0;--j)
                                curv[j][i] = curv[i][j];
                            continue;
                        }

                        // assuming not used by algorithm
                        //beta[i] = zero;
                        //curv.ldCol(i,zero);
                        //curv.ldRow(i,zero);
                        curv[i][i] = one;
                    }

                    //----------------------------------------------------------
                    // return sim
                    //----------------------------------------------------------
                    Coerce(npts) = np;
                    return ( Coerce(last) = half * xadd.sum() );
                }

                //______________________________________________________________
                //
                //
                // Methods for multiple samples
                //
                //______________________________________________________________

                //! summing precomputed LeastSquares
                inline ABSCISSA Of(const List &L)
                {

                    if(L.size<=0) return zero;

                    //----------------------------------------------------------
                    // prepare memory
                    //----------------------------------------------------------
                    {
                        const LeastSquares &first = *(L.head);
                        const size_t nv = first.dFda.size(); assert(nv>0);
                        dFda.adjust(nv,zero);
                        beta.adjust(nv,zero);
                        curv.make(nv,nv);
                        xadd.make(L.size);
                    }

                    //----------------------------------------------------------
                    // first pass: npts
                    //----------------------------------------------------------
                    Coerce(npts) = 0;
                    for(const LeastSquares *cls = L.head; cls; cls=cls->next)
                    {
                        Coerce(npts) += cls->npts;
                    }

                    //----------------------------------------------------------
                    // second pass: beta
                    //----------------------------------------------------------
                    const size_t   nvar = beta.size();
                    const ABSCISSA den  = ABSCISSA(npts);
                    for(size_t i=nvar;i>0;--i)
                    {
                        xadd.free();
                        for(const LeastSquares *cls = L.head; cls; cls=cls->next)
                        {
                            xadd << (cls->beta[i]* static_cast<ABSCISSA>(cls->npts) );
                        }
                        beta[i] = xadd.sum() / den;
                    }

                    //----------------------------------------------------------
                    // third pass: curv
                    //----------------------------------------------------------
                    for(size_t i=nvar;i>0;--i)
                    {
                        for(size_t j=i;j>0;--j)
                        {
                            xadd.free();
                            for(const LeastSquares *cls = L.head; cls; cls=cls->next)
                            {
                                xadd << (cls->curv[i][j]*static_cast<ABSCISSA>(cls->npts));
                            }
                            curv[i][j] = curv[j][i] = xadd.sum() / den;
                        }
                    }

                    //----------------------------------------------------------
                    // final D2
                    //----------------------------------------------------------
                    xadd.free();
                    for(const LeastSquares *cls = L.head; cls; cls=cls->next)
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
                XAdd                          xadd; //!< to perform additions
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

                    const size_t nv = beta.size();
                    for(size_t i=nv;i>0;--i)
                    {
                        if(!used[i]) 
                        {
                            assert( FabsOf(beta[i])<=zero) ;
                            continue;
                        }
                        const ABSCISSA *lhs = SampleType::O2A(dFda[i]);
                        for(unsigned d=0;d<Dimension;++d)
                        {
                            beta[i] += df[d] * lhs[d];
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

