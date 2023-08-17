
//! \file

#ifndef Y_CamEO_Add_Heap_Included
#define Y_CamEO_Add_Heap_Included 1

#include "y/ordered/heap.hpp"
#include "y/ordered/core/flexible-raw-buffer.hpp"
#include "y/ordered/core/compiled-raw-buffer.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Cameo
        {

            //__________________________________________________________________
            //
            //
            //
            //! Policy to use an internal heap to perform additions
            //
            //
            //__________________________________________________________________
            template <typename T>
            struct HeapPolicy
            {
                //______________________________________________________________
                //
                //
                //! comparator of absValue of Units
                //
                //______________________________________________________________
                class  Comparator
                {
                public:
                    inline  Comparator() noexcept {} //!< setup
                    inline ~Comparator() noexcept {} //!< cleanup

                    //! decreasing absolute value for heap
                    inline SignType operator()(const T &lhs, const T &rhs) const
                    { return Comparison::CxxDecreasing(lhs.absValue,rhs.absValue); }
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Comparator);
                };

                //______________________________________________________________
                //
                //
                //! Flexible Units for a given ALLOCATOR
                //
                //______________________________________________________________
                template <typename ALLOCATOR>
                struct FlexibleUnits
                {
                    typedef Heap<T,Core::FlexibleRawBuffer<T,ALLOCATOR>,Comparator > Type; //!< alias
                };

                //______________________________________________________________
                //
                //
                //! Compile Units for a given size
                //
                //______________________________________________________________
                template <size_t N>
                struct CompiledUnits
                {
                    typedef Heap<T,Core::CompiledRawBuffer<N,T>,Comparator> Type; //!< alias
                };

                //______________________________________________________________
                //
                //
                //! Reduction os a Heap of Units whit more than one item
                //
                //______________________________________________________________
                template <typename UNITS> static inline
                T Reduce(UNITS &units)
                {
                    assert(units.size()>1);
                    while(units.size()>1)
                    {
                        const T lhs = units.pull();
                        const T rhs = units.pull();
                        const T sum = lhs+rhs;
                        units.push(sum);
                    }
                    assert(1==units.size());
                    return units.pull();
                }
                
                
            };
            
        }
    }

}


#endif

