
#include "y/mkl/filter/resago.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/ptr/ark.hpp"
#include "y/associative/hash/set.hpp"
#include "y/system/exception.hpp"
#include "y/calculus/ipower.hpp"
#include <cstring>

namespace Yttrium
{
    namespace MKL
    {

        namespace
        {

            //__________________________________________________________________
            //
            //! alias for solver
            //__________________________________________________________________
            typedef LU<apq> ReSaGoSolver;


            //__________________________________________________________________
            //
            //
            //! Metrics to compute filter coefficients
            //
            //__________________________________________________________________
            class ReSaGoMetrics
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline  ReSaGoMetrics(const int32_t  n_,
                                      const int32_t  j_,
                                      const uint32_t d_) noexcept :
                n(n_),
                j(j_),
                d(d_)
                {
                    assert(n>0);
                    assert(j>=1);
                    assert(j<=n);
                    assert(d<size_t(n));
                }

                //! cleanup
                inline ~ReSaGoMetrics() noexcept {}

                //! copy
                inline  ReSaGoMetrics(const ReSaGoMetrics &other) noexcept :
                n(other.n), j(other.j), d(other.d)
                {
                }

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                const int32_t  n; //!< number of points n>=1
                const int32_t  j; //!< evaluation in [1:n]
                const uint32_t d; //!< degree < n

            private:
                Y_DISABLE_ASSIGN(ReSaGoMetrics);
            };


            //__________________________________________________________________
            //
            //
            //! Compound Key for Hash Table
            //
            //__________________________________________________________________
            class ReSaGoKey : public Memory::ReadOnlyBuffer
            {
            public:
                static const char * const CallSign; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline explicit ReSaGoKey(const int32_t  n,
                                          const int32_t  j,
                                          const uint32_t d) :
                metrics(CheckN(n),CheckJ(j,n),CheckD(d,n))
                {
                }

                //! copy
                inline explicit ReSaGoKey(const ReSaGoKey &other) noexcept :
                Memory::ReadOnlyBuffer(),
                metrics(other.metrics)
                {
                }


                //! cleanup
                inline virtual ~ReSaGoKey() noexcept {}

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________
                virtual const void * ro_addr() const noexcept { return &metrics;        }
                virtual size_t       measure() const noexcept { return sizeof(metrics); }

                inline friend bool operator==(const ReSaGoKey &lhs, const ReSaGoKey &rhs) noexcept
                {
                    return 0 == memcmp(&lhs.metrics,&rhs.metrics,sizeof(ReSaGoMetrics));
                }

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                const ReSaGoMetrics metrics; //!< POD metrics

            private:
                Y_DISABLE_ASSIGN(ReSaGoKey);

                static inline int32_t CheckN(const int32_t n)
                {
                    if(n<=0) throw Specific::Exception(CallSign,"n<=0");
                    return n;
                }

                static inline int32_t CheckJ(const int32_t j, const int32_t n)
                {
                    if(j<=0||j>n) throw Specific::Exception(CallSign,"j=%d not in [1:%d]",j,n);
                    return j;
                }

                static uint32_t CheckD(const uint32_t d, const int32_t n)
                {
                    assert(n>0);
                    if(d>=uint32_t(n)) throw Specific::Exception(CallSign,"degree=%u>=%d",d,n);
                    return d;
                }

            };

            const char * const ReSaGoKey:: CallSign = "ReSaGo::Metrics";


            //__________________________________________________________________
            //
            //
            //! Filter for a given Metrics
            //
            //__________________________________________________________________
            class ReSaGoFilter : public Object, public Counted, public ReSaGoKey
            {
            public:
                static const char * const CallSign;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit ReSaGoFilter(ReSaGoSolver    &lu,
                                             const ReSaGoKey &fk) :
                ReSaGoKey(fk),
                p(metrics.d+1),
                f(Min(p,1+ReSaGo::MaxKeptDegree),metrics.n)
                {
                    //----------------------------------------------------------
                    //
                    // compute moments matrix
                    //
                    //----------------------------------------------------------
                    Matrix<apq> mu(p,p);
                    const int32_t n = metrics.n;
                    const int32_t j = metrics.j;
                    for(size_t l=1,lm=0;l<=p;++l,++lm)
                    {
                        for(size_t k=1,km=0;k<=l;++k,++km)
                        {
                            apz sum = 0;
                            for(int32_t i=1;i<=n;++i)
                            {
                                const apz imj    = i-j;
                                const apz imj_lk = ipower(imj,lm+km);
                                sum += imj_lk;
                            }
                            mu[k][l]= mu[l][k] = sum;
                        }
                    }
                    //std::cerr << "mu=" << mu << std::endl;
                    if(!lu.build(mu)) throw Exception(CallSign,"unexpected singular moments");

                    //----------------------------------------------------------
                    //
                    // compute right hand side
                    //
                    //----------------------------------------------------------
                    Matrix<apq> F(p,n);
                    for(size_t l=1,lm=0;l<=p;++l,++lm)
                    {
                        for(int32_t i=1;i<=n;++i)
                        {
                            const apz imj   = i-j;
                            const apz imj_l = ipower(imj,lm);
                            F[l][i] = imj_l;
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // extract coefficients up to desired ordere
                    //
                    //----------------------------------------------------------
                    lu.solve(mu,F);
                    assert(F.rows>=f.rows);
                    assert(F.cols==f.cols);
                    for(size_t r=1;r<=f.rows;++r)
                    {
                        Writable<apq>       &f_r = Coerce(f[r]);
                        const Readable<apq> &F_r = F[r];
                        for(size_t c=1;c<=f.cols;++c)
                        {
                            f_r[c] = F_r[c];
                        }
                    }
                }

                //! cleanup
                inline virtual ~ReSaGoFilter() noexcept
                {
                }

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________

                //! key for Set
                const ReSaGoKey &    key()     const noexcept { return *this; }

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                const size_t       p; //!< 1+degree
                const Matrix<apq>  f; //!< filter [1..Min(p,MaxKeptDegree)][1..n]

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ReSaGoFilter);

            };

            const char * const ReSaGoFilter:: CallSign = "ReSaGo::Filter";


            typedef ArkPtr<ReSaGoKey,ReSaGoFilter>        ReSaGoSharedFilter; //!< alias
            typedef HashSet<ReSaGoKey,ReSaGoSharedFilter> ReSaGoSetOfFilters; //!< alias

            //__________________________________________________________________
            //
            //
            //! Database of filters
            //
            //__________________________________________________________________
            class ReSaGoFilters : public ReSaGoSetOfFilters
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline explicit ReSaGoFilters() noexcept :
                ReSaGoSetOfFilters(),
                lu()
                {}

                //! cleanup
                inline virtual ~ReSaGoFilters() noexcept {}

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________

                //! get/create
                const ReSaGoFilter & get(const ReSaGoKey &key)
                {
                    {
                        const ReSaGoSharedFilter *ppF = search(key);
                        if(ppF) return **ppF;
                    }

                    ReSaGoFilter *F = new ReSaGoFilter(lu,key);
                    {
                        const ReSaGoSharedFilter tmp(F);
                        if(!insert(tmp)) throw Specific::Exception("ReSaGoFilters","unexpected insert failure");
                    }
                    return *F;
                }



                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                ReSaGoSolver lu; //!< shared solver

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ReSaGoFilters);
            };

        }


        class ReSaGo::Factory::Code :
        public Object,
        public ReSaGoFilters
        {
        public:
            inline explicit Code() noexcept : Object(), ReSaGoFilters() {}
            inline virtual ~Code() noexcept {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        ReSaGo:: Factory::  Factory() : code( new Code() )
        {
            std::cerr << "sizeof(ReSaGoFilters)=" << sizeof(ReSaGoFilters) << std::endl;
        }

        ReSaGo:: Factory:: ~Factory() noexcept
        { assert(0!=code); Nullify( Coerce(code) ); }


        const Matrix<apq> & ReSaGo:: Factory:: operator()(const uint32_t nLeft,
                                                          const uint32_t nRight,
                                                          const uint32_t degree)
        {
            assert(0!=code);
            const int32_t j = nLeft+1;
            const int32_t n = j+nRight;
            if( degree >= uint32_t(n) ) throw Specific:: Exception("ReSaGo::Factory","degree=%u >= %d points", degree, n);
            const ReSaGoKey key(n,j,degree);
            return code->get(key).f;
        }

    }

}

#include "y/mkl/filter/savgol.hpp"
#include "y/system/rtti.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace
        {

            //! unique filter containing converted matrix
            template <typename T>
            class SavGolFilter : public Object, public Counted, public ReSaGoKey
            {
            public:
                typedef ArkPtr<ReSaGoKey,SavGolFilter> Pointer;
                typedef HashSet<ReSaGoKey,Pointer>     DataBase;

                // C++
                
                inline explicit SavGolFilter(const ReSaGoFilter &filter) :
                Object(),
                Counted(),
                ReSaGoKey(filter),
                f(filter.f.rows,filter.f.cols)
                {
                    for(size_t i=1;i<=f.rows;++i)
                    {
                        for(size_t j=1;j<=f.cols;++j)
                        {
                            const apq &q = filter.f[i][j];
                            
                        }
                    }
                }

                inline virtual ~SavGolFilter() noexcept {}

                // Methods
                inline const ReSaGoKey & key() const noexcept { return *this; }


                // Members
                const Matrix<T> f;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(SavGolFilter);
            };
        }

        template <typename T>
        class SavGol<T>::Code :
        public Object,
        public SavGolFilter<T>::DataBase
        {
        public:
            typedef SavGolFilter<T>                   FilterType;
            typedef typename SavGolFilter<T>::Pointer FilterPointer;

            inline explicit Code(const SavGolFactory &sharedFactory) :
            Object(),
            factory( sharedFactory    ),
            filters( *(factory->code) ),
            tid( RTTI::Name<T>() )
            {
            }

            inline virtual ~Code() noexcept {}

            inline
            const FilterType &get(const int32_t  n,
                                  const int32_t  j,
                                  const uint32_t d)
            {
                const ReSaGoKey key(n,j,d);
                {
                    const FilterPointer *ppF = this->search(key);
                    if(ppF) return **ppF;
                }

                const ReSaGoFilter &impl = filters.get(key);
                FilterType         *F    = new FilterType(impl);
                {
                    const FilterPointer tmp(F);
                    if(!this->insert(tmp)) throw Specific::Exception("SavGol","unexpected insertion failure");
                }
                return *F;
            }


            SavGolFactory          factory;
            ReSaGo::Factory::Code &filters;
            const String          &tid;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

#define real_t float
#include "savgol.hxx"

#undef real_t
#define real_t double
#include "savgol.hxx"

    }
}
