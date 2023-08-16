//! \file

#ifndef Y_Ordered_Core_Compiled_Raw_Buffer_Included
#define Y_Ordered_Core_Compiled_Raw_Buffer_Included 1

#include "y/ordered/core/raw-buffer.hpp"
#include "y/calculus/align.hpp"

namespace Yttrium
{

    namespace Core
    {
        struct CompiledRawBuffer_
        {
            static void RaiseExceeded(const size_t n);
        };

        template <size_t N, typename T>
        class CompiledRawBuffer : public RawBuffer<T>
        {
        public:
            Y_ARGS_DECL(T,Type);
            using RawBuffer<T>::tally;
            using RawBuffer<T>::count;
            using RawBuffer<T>::entry;
            using RawBuffer<T>::free;

            explicit CompiledRawBuffer() noexcept :
            RawBuffer<T>() {
                Coerce(tally) = N;
                Coerce(entry) = Memory::OutOfReach::Cast<MutableType>( Y_STATIC_ZARR(wksp) );
            }


            virtual ~CompiledRawBuffer() noexcept
            { free(); }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(CompiledRawBuffer);
            static const size_t PrivateBytes = sizeof(T) * N;
            void *wksp[ Y_WORDS_GEQ(PrivateBytes) ];

            inline virtual void mustAcceptNext()
            { if(count>=tally) CompiledRawBuffer_::RaiseExceeded(tally); }

            inline virtual void prepareForMaxi(const size_t request)
            { assert(0==count); if(request>tally) CompiledRawBuffer_::RaiseExceeded(tally); }

        };

    }

}

#endif
