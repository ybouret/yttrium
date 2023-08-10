//! \file

#ifndef Y_Jive_Pattern_First_Chars_Included
#define Y_Jive_Pattern_First_Chars_Included 1

#include "y/config/starting.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    namespace Jive
    {
        class Pattern;

        //______________________________________________________________________
        //
        //
        //
        //! field of 256 bits for each possible first chars
        //
        //
        //______________________________________________________________________
        class FirstChars
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t Bits = 256;      //!< alias
            static const size_t Bytes = Bits/8;  //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit FirstChars();          //!< setup, all false
            virtual ~FirstChars() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(FirstChars);    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void include(const uint8_t b) noexcept; //!< [b] = true
            void exclude(const uint8_t b) noexcept; //!< [b] = false

            void include(const uint8_t a, const uint8_t b) noexcept; //!< include range
            void exclude(const uint8_t a, const uint8_t b) noexcept; //!< exclude range

            void includeAll() noexcept; //!< all bits to true
            void excludeAll() noexcept; //!< all bits to false

            void add(const FirstChars &fc) noexcept; //!< include active fc
            void sub(const FirstChars &fc) noexcept; //!< exclide active fc

            bool   operator[](const uint8_t) const noexcept; //!< query status

            Pattern *compile() const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FirstChars);
            uint8_t * const field;
        };
    }

}

#endif

