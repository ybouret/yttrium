
#include "y/concurrent/loop/simd.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/concurrent/split.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        Range::  Range() noexcept : length(0), offset(0) {}
        Range:: ~Range() noexcept {}

        std::ostream & operator<<(std::ostream &os, const Range &range)
        {
            os << "[@" << range.offset << "+" << range.length << "]";
            return os;
        }

        typedef CxxArray<const Range,Memory::Dyadic> CxxRanges;

        class SIMD:: Code : public Object, public CxxRanges
        {
        public:
            explicit Code(const Loop &loop) :
            Object(), CxxRanges(loop.size())
            {
            }

            inline virtual ~Code() noexcept {}



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        const char * const SIMD:: CallSign = "Concurrent::SIMD";

        SIMD:: ~SIMD() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        SIMD:: SIMD(const SharedLoop &team) :
        Readable<const Range>(),
        loop( team ),
        code( new Code(*loop) ),
        full()
        {
        }
        

        SIMD:: SIMD(Loop *team) :
        Readable<const Range>(),
        loop( team ),
        code( new Code(*loop) ),
        full()
        {
        }
       
        void SIMD:: dispatch(const size_t dataLength, const size_t dataOffset) noexcept
        {
            assert(0!=code);
            assert( loop->size() == code->size() );

            const Readable<const Range> &self  = *code;
            const size_t                 size_ = self.size();
            
            for(size_t rank=0,indx=1;rank<size_;++rank,++indx)
            {
                const Range &range       = self[indx];
                size_t      &rangeLength = (Coerce(range.length)=dataLength);
                size_t      &rangeOffset = (Coerce(range.offset)=dataOffset);
                Split::With(size_,rank,rangeLength,rangeOffset);
            }

            Coerce(full.offset) = dataOffset;
            Coerce(full.length) = dataLength;

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


        void SIMD:: CallMe:: operator()(const ThreadContext &ctx) const
        {
            const Range &range = ranges[ctx.indx];
            Y_LOCK(ctx.sync);
            (std::cerr << SIMD::CallSign << "::Range " << range << std::endl).flush();

        }


        void SIMD:: operator()(void) noexcept
        {
            const CallMe call = { *this };
            (*loop)(call);
        }

    }

}

