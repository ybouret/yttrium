//! \file

#ifndef Y_Ordered_Core_Compiled_Raw_Buffer_Included
#define Y_Ordered_Core_Compiled_Raw_Buffer_Included 1

#include "y/ordered/core/raw-buffer.hpp"
#include "y/calculus/align.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //
        //! helper for Compiled Raw Buffer
        //
        //
        //______________________________________________________________________
        struct CompiledRawBuffer_
        {
            //! throw exception upon overload
            static void RaiseExceeded(const size_t n);
        };


        //______________________________________________________________________
        //
        //
        //
        //! Compiled (fixed-sized) Raw Buffer
        //
        //
        //______________________________________________________________________
        template <size_t N, typename T>
        class CompiledRawBuffer : public RawBuffer<T>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t PrivateBytes = sizeof(T) * N;             //!< private bytes
            static const size_t PrivateWords = Y_WORDS_GEQ(PrivateBytes); //!< private words
            Y_ARGS_DECL(T,Type);                                          //!< aliases
            using RawBuffer<T>::tally;
            using RawBuffer<T>::count;
            using RawBuffer<T>::entry;
            using RawBuffer<T>::free;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            explicit CompiledRawBuffer() noexcept :
            RawBuffer<T>() {
                Coerce(tally) = N;
                Coerce(entry) = Memory::OutOfReach::Cast<MutableType>( Y_STATIC_ZARR(wksp) );
            }


            //! cleanup
            virtual ~CompiledRawBuffer() noexcept
            { free(); }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! get workspace size
            static inline size_t WorkspaceSize() noexcept
            {
                return sizeof(wksp);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CompiledRawBuffer);
            void *wksp[ PrivateWords ];

            inline virtual void mustAcceptNext()
            { if(count>=tally) CompiledRawBuffer_::RaiseExceeded(tally); }

            inline virtual void prepareForMaxi(const size_t request)
            { assert(0==count); if(request>tally) CompiledRawBuffer_::RaiseExceeded(tally); }

        };

    }

}

#endif
