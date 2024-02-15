//! \file

#ifndef  Y_MKL_Intervals_Included
#define  Y_MKL_Intervals_Included 1

#include "y/mkl/interval.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //
        //! Intervals
        //
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR = Memory::Pooled>
        class Intervals : public CxxArray< Interval<T>, ALLOCATOR>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Interval<T>                       IntervalType; //!< alias
            typedef CxxArray<IntervalType,ALLOCATOR>  AssemblyType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Intervals(const size_t dims) : AssemblyType(dims) {} //!< initialize to R^dims
            inline virtual ~Intervals() noexcept                              {} //!< cleanup
            inline Intervals(const Intervals &other) :                           //
            Identifiable(), Collection(), AssemblyType(other)                 {} //!< copy

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Intervals &domain)
            {
                os << '{';
                const size_t n = domain.size();
                if(n>0)
                {
                    os << domain[1];
                    for(size_t i=2;i<=n;++i)
                        os << " x " << domain[i];
                }
                os << '}';
                return os;
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! test each coordinate is in corresponding interval
            template <typename ARRAY> inline
            bool contains(ARRAY &arr) const noexcept
            {
                const Readable<IntervalType> &self = *this;
                assert(self.size() == arr.size() );
                for(size_t i=self.size();i>0;--i)
                {
                    if(!self[i].contains(arr[i])) return false;
                }
                return true;
            }


        private:
            Y_DISABLE_ASSIGN(Intervals);
        };
    }
}

#endif

