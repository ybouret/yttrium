//! \file
//! 
#ifndef Y_Ordered_Core_Flexible_Raw_Buffer_Included
#define Y_Ordered_Core_Flexible_Raw_Buffer_Included 1

#include "y/ordered/core/raw-buffer.hpp"
#include "y/memory/allocator.hpp"
#include "y/type/releasable.hpp"

namespace Yttrium
{

    namespace Core
    {

        //______________________________________________________________________
        //
        //
        //
        //! Flexible Raw Buffer
        //
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR>
        class FlexibleRawBuffer : public RawBuffer<T>, public Releasable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_DECL(T,Type);        //!< aliases
            using RawBuffer<T>::tally;
            using RawBuffer<T>::count;
            using RawBuffer<T>::entry;
            using RawBuffer<T>::free;

            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________

            //! setup empty
            explicit FlexibleRawBuffer() noexcept :  RawBuffer<T>(), bytes(0) {}

            //! cleanup
            virtual ~FlexibleRawBuffer() noexcept { release_(); }

            //! setup with capacity
            explicit FlexibleRawBuffer(size_t n)  :  RawBuffer<T>(), bytes(0) { setup(n); }

            //! [Releasable] free and release memory
            virtual void release() noexcept
            {
                release_();
            }
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(FlexibleRawBuffer);
            const size_t bytes;

            inline void release_() noexcept
            {
                free();
                Release(Coerce(entry),Coerce(tally),Coerce(bytes));
            }

            static inline void Release(MutableType * & entry,
                                       size_t        & count,
                                       size_t        & bytes) noexcept
            {
                assert(Good(entry,count));
                if(entry)
                {
                    static Memory::Allocator &mgr = ALLOCATOR::Location();
                    mgr.withdraw(entry,count,bytes);
                }
            }

            void setup(const size_t request)
            {
                assert(0==entry); assert(0==count); assert(0==tally); assert(0==bytes);
                if(request>0)
                {
                    static Memory::Allocator &mgr = ALLOCATOR::Instance();
                    Coerce(entry) = mgr.allocate<MutableType>( Coerce(tally) = request, Coerce(bytes) );
                }
            }

            inline void upgradeTally(const size_t request)
            {
                assert(request>tally);
                static Memory::Allocator &mgr = ALLOCATOR::Instance();
                size_t       newTally = request;
                size_t       newBytes = 0;
                MutableType *newEntry = mgr.allocate<MutableType>( newTally, newBytes);
                Memory::OutOfReach::Copy(newEntry,entry,count*sizeof(T));
                Release( Coerce(entry), Coerce(tally), Coerce(bytes) );
                Coerce(entry) = newEntry;
                Coerce(tally) = newTally;
                Coerce(bytes) = newBytes;
            }

            virtual void mustAcceptNext()
            {
                if(count>=tally)
                    upgradeTally( Container::NextCapacity(tally) );
            }

            virtual void prepareForMaxi(const size_t request)
            {
                assert(0==count);
                if(request>tally) upgradeTally( request );
            }
        };
    }

}

#endif

