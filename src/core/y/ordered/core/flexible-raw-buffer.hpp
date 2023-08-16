//! \file
//! 
#ifndef Y_Ordered_Core_Flexible_Raw_Buffer_Included
#define Y_Ordered_Core_Flexible_Raw_Buffer_Included 1

#include "y/ordered/core/raw-buffer.hpp"
#include "y/memory/allocator.hpp"

namespace Yttrium
{

    namespace Core
    {
        template <typename T, typename ALLOCATOR>
        class FlexibleRawBuffer : public RawBuffer<T>
        {
        public:
            Y_ARGS_DECL(T,Type);
            using RawBuffer<T>::tally;
            using RawBuffer<T>::count;
            using RawBuffer<T>::entry;
            using RawBuffer<T>::free;
            explicit FlexibleRawBuffer() noexcept :  RawBuffer<T>(), bytes(0) {}
            virtual ~FlexibleRawBuffer() noexcept { release(); }
            explicit FlexibleRawBuffer(size_t n)  :  RawBuffer<T>(), bytes(0) { setup(n); }

            virtual void release() noexcept
            {
                free();
                Release(Coerce(entry),Coerce(tally),Coerce(bytes));
            }

            inline void xch(FlexibleRawBuffer &other) noexcept
            {
                CoerceSwap(entry,other.entry);
                CoerceSwap(count,other.count);
                CoerceSwap(tally,other.tally);
                CoerceSwap(bytes,other.bytes);
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(FlexibleRawBuffer);
            const size_t bytes;

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

            inline void upgradeMaxi(const size_t request)
            {
                assert(request>=tally);
                FlexibleRawBuffer buff( request ); assert(buff.tally>tally);
                Memory::OutOfReach::Copy(buff.entry,entry,(buff.count=count)*sizeof(T));
                count=0;
                xch(buff);
            }

            virtual void mustAcceptNext()
            {
                if(count>=tally)
                    upgradeMaxi( Container::NextCapacity(tally) );
            }

            virtual void prepareForMaxi(const size_t request)
            {
                assert(0==count);
                if(request>tally) upgradeMaxi( request );
            }
        };
    }

}

#endif

