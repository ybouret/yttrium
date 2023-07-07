// \file

#ifndef Y_Random_Bits_Included
#define Y_Random_Bits_Included 1

#include "y/calculus/align.hpp"

namespace Yttrium
{
    namespace Random
    {


        //! Random Bits interface
        class Bits
        {
        public:
            template <typename T> class Engine;
            static const size_t         EngineBytes = 48;

            explicit Bits(const uint32_t umax) noexcept;
            virtual ~Bits() noexcept;

            virtual uint32_t next32() noexcept = 0;

            template <typename T> T to() noexcept;

            const Engine<float>       * const F;
            const Engine<double>      * const D;
            const Engine<long double> * const L;
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bits);
            void *wkspF[ Y_WORDS_GEQ(EngineBytes)  ];
            void *wkspD[ Y_WORDS_GEQ(EngineBytes)  ];
            void *wkspL[ Y_WORDS_GEQ(EngineBytes)  ];
        };

        class Rand : public Bits
        {
        public:
            explicit Rand() noexcept;
            virtual ~Rand() noexcept;

            virtual uint32_t next32() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Rand);
        };


    }

}

#endif

