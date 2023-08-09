//! \file

#ifndef Y_Jive_Pattern_Included
#define Y_Jive_Pattern_Included 1

#include "y/jive/token.hpp"

namespace Yttrium
{
    namespace Jive
    {
        class FirstChars
        {
        public:
            static const size_t Bits = 256;
            static const size_t Bytes = Bits/8;

            explicit FirstChars();
            virtual ~FirstChars() noexcept;
            Y_OSTREAM_PROTO(FirstChars);

            void include(const uint8_t b) noexcept;
            void exclude(const uint8_t b) noexcept;

            void include(const uint8_t a, const uint8_t b) noexcept;
            void exclude(const uint8_t a, const uint8_t b) noexcept;

            void includeAll() noexcept;
            void excludeAll() noexcept;

            bool   operator[](const uint8_t) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FirstChars);
            uint8_t * const field;
        };
    }

}

#endif

