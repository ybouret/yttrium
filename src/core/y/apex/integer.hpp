
//! \file

#ifndef Y_Apex_Integer_Included
#define Y_Apex_Integer_Included 1

#include "y/apex/natural.hpp"

namespace Yttrium
{

    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! Integer = sign + natural
        //
        //
        //______________________________________________________________________
        class Integer : public Number
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const char * const CallSign;
            static const uint8_t Is__Zero__ = 0x00;
            static const uint8_t IsPositive = 0x01;
            static const uint8_t IsNegative = 0x02;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Integer(const integer_t z);
            Integer(const Integer &);
            Integer(const Natural &);
            Integer(const SignType, const Natural &);
            Integer & operator=(const Integer  &);
            Integer & operator=(const integer_t );
            Integer & operator=(const Natural  &);
            virtual ~Integer() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Integer & xch( Integer & ) noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()       const noexcept;
            virtual size_t       serialize(OutputStream &) const;
            static  Integer      Read(InputStream &);

            //__________________________________________________________________
            //
            //
            // comparison
            //
            //__________________________________________________________________
            friend bool operator==(const Integer &lhs, const Integer &rhs) noexcept;
            friend bool operator!=(const Integer &lhs, const Integer &rhs) noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SignType s; //!< sign
            const Natural  n; //!< synchronized natural
        };

    }

}

#endif

