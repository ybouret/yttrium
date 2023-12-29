#include "y/mkl/filter/smooth.hpp"
#include "y/mkl/statistics/metrics.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/container/cxx/array.hpp"

#include "y/type/nullify.hpp"

#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/calculus/ipower.hpp"
#include "y/system/exception.hpp"
#include <cerrno>

#include "y/sequence/vector.hpp"

#include "y/associative/little-endian-key.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace MKL
    {



        typedef Memory::Dyadic  MemoryModel; //!< alias
        typedef LittleEndianKey KeyType;     //!< alias

        //__________________________________________________________________
        //
        //
        //! Matrix + Vector of given dimension
        //
        //__________________________________________________________________
        template <typename T>
        class Moments : public Object, public Counted
        {
        public:
            //______________________________________________________________
            //
            //
            // Definitions
            //
            //______________________________________________________________
            typedef ArkPtr<KeyType,Moments>    Pointer;
            typedef SuffixSet<KeyType,Pointer> DataBase;

            //______________________________________________________________
            //
            //
            // C++
            //
            //______________________________________________________________

            //! acquire resources
            inline explicit Moments(const size_t n) :
            mu(n,n),
            cf(n),
            k_(n)
            {
            }

            //! cleanup
            inline virtual ~Moments() noexcept {}

            //______________________________________________________________
            //
            //
            // Methods
            //
            //______________________________________________________________

            //! for database
            inline const KeyType & key() const noexcept { return k_; }

            //______________________________________________________________
            //
            //
            // Members
            //
            //______________________________________________________________
            Matrix<T>               mu; //!< matrix of moment
            CxxArray<T,MemoryModel> cf; //!< right hand side

        private:
            const LittleEndianKey   k_;
            Y_DISABLE_COPY_AND_ASSIGN(Moments);
        };



        //! Code for Smooth
        template <typename T>
        class Smooth<T> :: Code : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // Defintions
            //
            //__________________________________________________________________
            typedef Vector<T,MemoryModel>         Tableau;     //!< alias
            typedef Antelope::Add<T>              XAdd;        //!< alias
            typedef CxxArray<T,MemoryModel>       Array;       //!< alias
            typedef Moments<T>                    MomentsType; //!< alias
            typedef typename Moments<T>::Pointer  MomentsPtr;  //!< alias
            typedef typename Moments<T>::DataBase MomentsDB;   //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            inline explicit Code() :
            zero(0),
            coef(4,zero),
            solv(),
            ttab(),
            ztab(),
            xadd(),
            mydb()
            {}

            //! cleanup
            inline virtual ~Code() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! get/create moments of size m
            //__________________________________________________________________
            inline MomentsType & get(const size_t m)
            {
                const KeyType key(m);
                {
                    MomentsPtr * ppM = mydb.search(key);
                    if(0!=ppM)
                        return **ppM;
                }

                MomentsType *M = new MomentsType(m);
                {
                    const MomentsPtr tmp(M);
                    if(!mydb.insert(tmp))
                        throw Specific::Exception("Smooth::Moments","unexpected insertion failure");
                }
                return *M;
            }


            //__________________________________________________________________
            //
            //! reserve moments for 0..degree
            //__________________________________________________________________
            inline void reserveMaxDegree(const size_t degree)
            {
                const size_t top = degree+1;
                solv.ensure(top);
                for(size_t m=top;m>0;--m) (void) get(m);

            }


            //__________________________________________________________________
            //
            //! reserve moments for 0..degree
            //__________________________________________________________________
            inline void reserveMaxLength(const size_t points)
            {
                ztab.free();
                ttab.free();
                ttab.ensure(points);
                ztab.ensure(points);
                xadd.make(points);
            }

            //__________________________________________________________________
            //
            //! process selection
            //__________________________________________________________________
            inline void run(const T           &t0,
                            const Readable<T> &t,
                            const Readable<T> &z,
                            const size_t       degree)
            {
                assert(t.size()==z.size());

                //--------------------------------------------------------------
                // prepare resources
                //--------------------------------------------------------------
                const size_t n = t.size(); if(n<=0) return;
                xadd.make(n);
                ttab.adjust(n,zero);
                ztab.adjust(n,zero);

                const size_t m = Min(degree+1,n);
                MomentsType &moments = get(m);
                solv.ensure(m);



                //--------------------------------------------------------------
                // initialize internal coef
                //--------------------------------------------------------------
                coef.ld(zero);

                //--------------------------------------------------------------
                // center t
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i)
                {
                    ttab[i] = t[i] - t0;
                    xadd << z[i];
                }

                //--------------------------------------------------------------
                // center z
                //--------------------------------------------------------------
                const T z0 = xadd.sum()/T(n);
                for(size_t i=n;i>0;--i)
                {
                    ztab[i] = z[i] - z0;
                }

                //--------------------------------------------------------------
                // compute matrix coefficients
                //--------------------------------------------------------------
                Matrix<T>   &mu = moments.mu; assert(mu.rows==m); assert(mu.cols==m);
                for(size_t i=1;i<=m;++i)
                {
                    for(size_t j=i;j<=m;++j)
                    {
                        const size_t p = (j+i)-2;
                        xadd.free();
                        for(size_t k=n;k>0;--k)
                        {
                            const T tmp = ipower(ttab[k],p);
                            xadd << tmp;
                        }
                        mu[i][j] = mu[j][i] = xadd.sum();
                    }
                }
                if(!solv.build(mu)) throw Libc::Exception(EDOM,"singular smoothing moments");

                //--------------------------------------------------------------
                // compute right hand side
                //--------------------------------------------------------------
                Writable<T> &cf = moments.cf; assert(m==cf.size());
                cf[1] = zero;
                for(size_t j=2;j<=m;++j)
                {
                    xadd.free();
                    for(size_t k=n;k>0;--k)
                    {
                        const T tmp = ipower(ttab[k],j-1);
                        xadd << tmp * ztab[k];
                    }
                    cf[j] = xadd.sum();
                }

                //--------------------------------------------------------------
                // compute all coefficients
                //--------------------------------------------------------------
                solv.solve(mu,cf);


                //--------------------------------------------------------------
                // transfer coefficients
                //--------------------------------------------------------------
                {        coef[1] = cf[1] + z0; }
                if(m>=2) coef[2] = cf[2];
                if(m>=3) coef[3] = cf[3]+cf[3];

            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const T          zero; //!< constant
            Array            coef; //!< allocated for Smooth
            LU<T>            solv; //!< solver
            Tableau          ttab; //!< centered t
            Tableau          ztab; //!< centered z
            XAdd             xadd; //!< local xadd
            MomentsDB        mydb; //!< database of moments

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

#define real_t float
#include "smooth.hxx"
#undef real_t

#define real_t double
#include "smooth.hxx"
#undef real_t

#define real_t long double
#include "smooth.hxx"
#undef real_t

#define real_t XReal<float>
#include "smooth.hxx"
#undef real_t

#define real_t XReal<double>
#include "smooth.hxx"
#undef real_t

#define real_t XReal<long double>
#include "smooth.hxx"
#undef real_t

    }

}


