
//! \file

#ifndef Y_Ink_LUT8__Included
#define Y_Ink_LUT8__Included 1

#include "y/ink/pixmap.hpp"

namespace Yttrium
{
    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! 8-bits Look Up Table
        //
        //
        //______________________________________________________________________
        class LookUpTable8
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned SIZE = 256;    //!< table size
            static const uint8_t  Default[SIZE]; //!< default table

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            LookUpTable8()                                 noexcept; //!< setup to Default
            ~LookUpTable8()                                noexcept; //!< cleanup
            LookUpTable8(const LookUpTable8 &)             noexcept; //!< copy
            LookUpTable8 & operator=(const LookUpTable8 &) noexcept; //!< assign

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            uint8_t operator[](const uint8_t) const noexcept; //!< access

            void reset()   noexcept; //!< reset to Default
            void reverse() noexcept; //!< reverse table

            //! build an expansion table
            /**
             - i <= lower => 0x00
             - i >= upper => 0xff
             */
            void stretch(const uint8_t lower, const uint8_t upper) noexcept;

            //! apply substitution
            void operator()(Slabs                 &slabs,
                            Pixmap<uint8_t>       &target,
                            const Pixmap<uint8_t> &source);

            //! apply substitution, in place
            void operator()(Slabs           &slabs,
                            Pixmap<uint8_t> &target);


        private:
            uint8_t data[SIZE];

            static void Apply(Slab &slab, Pixmap<uint8_t> &target, const LookUpTable8 &table, const Pixmap<uint8_t> &source);
        };

    }

}

#endif
