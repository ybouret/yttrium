
//! \file


#ifndef Y_Apex_Batch_Included
#define Y_Apex_Batch_Included 1

#include "y/calculus/base2.hpp"
#include "y/calculus/ilog2.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Nexus
        {
            //! anonymous memory operations
            struct Batch
            {
                static void * Acquire(unsigned &dataShift);                        //!< return 2^dataShift bytes
                static void   Release(void *entry, const unsigned shift) noexcept; //!< release previously allocated block
            };
        }

        //______________________________________________________________________
        //
        //
        //
        //! local batch of objects
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Batch
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Nexus::Batch Self; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! allocate next power of two numObjects
            inline Batch(const size_t numObjects) :
            count(     numObjects                                   ),
            shift(     Base2<size_t>::LogFor( Coerce(count))        ),
            dataShift( DataShiftFor(count)                          ),
            addr( static_cast<T*>(Self::Acquire(Coerce(dataShift))) )
            {

            }

            //! setup WITHOUT memory copy, just copy metrics
            inline Batch(const Batch &other) :
            count( other.count ),
            shift( other.shift ),
            dataShift( other.dataShift ),
            addr( static_cast<T*>(Self::Acquire(Coerce(dataShift))) )
            {

            }

            //! cleanup
            inline ~Batch() noexcept
            {
                assert(0!=addr);
                Self::Release(addr,dataShift);
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! access
            inline T & operator[](const size_t i) noexcept
            {
                assert(i<count);
                return addr[i];
            }

            //! access
            inline T *operator()(void) noexcept
            {
                return addr;
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t   count;     //!< 2^shift object
            const unsigned shift;     //!< log2(count)
            const unsigned dataShift; //!< required bytes = 2^dataShift

        private:
            Y_DISABLE_ASSIGN(Batch);
            T       *addr;

            static inline unsigned DataShiftFor( const size_t nobj )
            {
                size_t request = nobj * sizeof(T);
                return Base2<size_t>::LogFor(request);
            }
        };
    }
}

#endif
