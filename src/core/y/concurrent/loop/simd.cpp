
#include "y/concurrent/loop/simd.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/concurrent/split.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        Range::  Range() noexcept : length(0), offset(0) {}
        Range:: ~Range() noexcept {}

        typedef CxxArray<const Range,Memory::Dyadic> Ranges;

        class SIMD:: Code : public Object, public Ranges
        {
        public:
            explicit Code(const Loop &loop) :
            Object(), Ranges(loop.size())
            {
            }

            inline virtual ~Code() noexcept {}



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        const char * const SIMD:: CallSign = "Concurrent::SIMD";

        SIMD:: ~SIMD() noexcept
        {
        }

        SIMD:: SIMD(const SharedLoop &team) :
        Readable<const Range>(), Range(),
        loop( team ),
        code( new Code(*loop) )
        {
        }
        

        SIMD:: SIMD(Loop *team) :
        Readable<const Range>(), Range(),
        loop( team ),
        code( new Code(*loop) )
        {
        }
       
        size_t SIMD:: dispatch(const size_t dataLength, const size_t dataOffset) noexcept
        {
            assert(0!=code);
            assert( loop->size() == code->size() );

            const Readable<const Range> &self  = *code;
            const size_t                 size_ = self.size();
            size_t                       ans   = 0;
            
            for(size_t rank=0,indx=1;rank<size_;++rank,++indx)
            {
                const Range &range       = self[indx];
                size_t      &rangeLength = (Coerce(range.length)=dataLength);
                size_t      &rangeOffset = (Coerce(range.offset)=dataOffset);
                Split::With(size_,rank,rangeLength,rangeOffset);
                if(rangeLength>0) ++ans;
            }
            
            return ans;
        }

        size_t SIMD:: size() const noexcept
        {
            assert(0!=code);
            assert( loop->size() == code->size() );
            return loop->size();
        }

        const char * SIMD:: callSign() const noexcept
        {
            return CallSign;
        }

        const Range & SIMD:: operator[](const size_t indx) const noexcept
        {
            assert(0!=code);
            return (*code)[indx];
        }

    }

}

