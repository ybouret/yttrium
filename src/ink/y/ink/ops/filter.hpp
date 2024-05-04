//! \file

#ifndef Y_Ink_Filter_Included
#define Y_Ink_Filter_Included 1

#include "y/ink/pixmap.hpp"
#include "y/ink/types.hpp"
#include "y/string.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/sort/merge.hpp"
#include "y/container/cxx/series.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/color/channels.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{


    namespace Ink
    {

        namespace Crux
        {
            //__________________________________________________________________
            //
            //
            //! base class for Filter
            //
            //__________________________________________________________________
            class Filter : public Object, public Counted
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
              
                //! setup
                template <typename NAME> inline
                explicit Filter(const NAME &id) : name(id) {}

                //! cleanup
                virtual ~Filter() noexcept;

                //______________________________________________________________
                //
                //
                // Members+
                //
                //______________________________________________________________
                const String name; //!< identifier
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Filter);
            };

            //__________________________________________________________________
            //
            //
            //! horizontal factor
            //
            //__________________________________________________________________
            template <typename T>
            class Factor
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Small::BareHeavyList<Factor> List; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline  Factor(const unit_t X, const T W) noexcept : x(X), w(W) {} //!< setup
                inline  Factor(const Factor &F) noexcept : x(F.x), w(F.w) {}       //!< copy
                inline ~Factor() noexcept {}                                       //!< cleanup

                //! display
                inline friend std::ostream & operator<<(std::ostream &os, const Factor &self)
                {
                    os << self.w << "@[" << self.x << "]";
                    return os;
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const unit_t x; //!< horizontal offset
                const T      w; //!< weight
            private:
                Y_DISABLE_ASSIGN(Factor);
            };

            //__________________________________________________________________
            //
            //
            //! list of horizontal factors at a vertical offset
            //
            //__________________________________________________________________
            template <typename T>
            class Factors : public Factor<T>::List
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef typename Factor<T>::List ListType; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline explicit Factors(const unit_t Y) noexcept : y(Y) {}                     //!< setup
                inline          Factors(const Factors &other) : ListType(other), y(other.y) {} //!< copy
                inline virtual ~Factors() noexcept {}                                          //!< cleanup

                //! display
                inline friend std::ostream & operator<<(std::ostream &os, const Factors &self)
                {
                    const ListType &l = self;
                    os << "@[" << self.y << "]:" << l;
                    return os;
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const unit_t y; //!< vertical offset
            private:
                Y_DISABLE_ASSIGN(Factors);
            };

        }

        //______________________________________________________________________
        //
        //
        //
        //! Filter for a scalar pixmap
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Filter : public Crux::Filter
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Crux::Factor<T>                Factor;   //!< alias
            typedef Crux::Factors<T>               Factors;  //!< alias
            typedef typename Factors::NodeType     FNode;    //!< alias
            typedef CxxSeries<Factors,MemoryModel> HFactors; //!< alias
            typedef ArkPtr<String,const Filter>    Handle;   //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
          
            //! setup
            /**
             \param id     name of filter
             \param F      linear coefficients
             \param layout layout of F[h][w], with (x,y) = offset
             */
            template <
            typename NAME,
            typename U>
            explicit Filter(const NAME &       id,
                            const U    * const F,
                            const Area         layout) :
            Crux::Filter(id),
            norm(1),
            hfac(layout.h)
            {
                assert(0!=F);
                MKL::Antelope::Add<T> xadd(layout.n);
                const U * f = F;
                for(unit_t j=0;j<layout.h;++j,f += layout.w)
                {
                    Factors factors(j-layout.y);
                    for(unit_t i=0;i<layout.w;++i)
                    {
                        const U w = f[i];
                        if(0!=w)
                        {
                            const Factor factor(i-layout.x,w);
                            xadd    << w*w;
                            factors << factor;
                        }
                    }
                    //std::cerr << factors << std::endl;
                    if(factors.size>0)
                        hfac << factors;
                }
                if( xadd.size() > 0 )
                {
                    const T sq = xadd.sum();
                    Coerce(norm) = sqrt(sq);
                }
            }

            virtual ~Filter() noexcept {}

            

            //__________________________________________________________________
            //
            //! apply filter at one point and return the value
            //__________________________________________________________________
            template <typename U, typename V> inline
            const U apply(U               &target,
                          const Pixmap<V> &source,
                          const Coord     &origin) const
            {
                T sum(0);
                for(size_t j=hfac.size();j>0;--j)
                {
                    const Factors   &factors = hfac[j];
                    const PixRow<V> &src     = source[factors.y+origin.y];
                    for(const FNode *node=factors.head;node;node=node->next)
                    {
                        const Factor &f = **node;
                        sum += f.w * static_cast<T>(src[origin.x+f.x]);
                    }
                }
                return (target = static_cast<U>(sum));
            }

            //__________________________________________________________________
            //
            //! apply filter over slabs and collect min/max
            //__________________________________________________________________
            template <typename U, typename V> inline
            void operator()(Slabs           &slabs,
                            Pixmap<U>       &target,
                            U               &umin,
                            U               &umax,
                            const Pixmap<V> &source)
            {
                slabs( Apply<U,V>, target, *this, source );
                slabs.getMinMax(umin,umax);
            }

            const T  norm; //!< norm of coefficients
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Filter);
            HFactors hfac;

            //! apply and scan Min/Max into slab
            template <typename U, typename V> static inline
            void Apply(Slab &slab, Pixmap<U> &target, const Filter &F, const Pixmap<V> &source)
            {
                for(size_t k=slab.count();k>0;--k)
                {
                    const Ink::HSegment     seg = slab.hseg[k];
                    Coord                   pos(seg.x,seg.y);
                    PixRow<U>              &tgt = target[pos.y];
                    for(unit_t i=seg.w;i>0;--i,++pos.x)
                    {
                        F.apply(tgt[pos.x],source,pos);
                    }
                }
                slab.scanMinMax(target);
            }
        };

    }

}

#endif

