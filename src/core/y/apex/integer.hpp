
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
            static const char * const CallSign;        //!< Apex::Integert
            static const uint8_t Is__Zero__ = 0x00;    //!< alias
            static const uint8_t IsPositive = 0x01;    //!< alias
            static const uint8_t IsNegative = 0x02;    //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Integer(const integer_t z);               //!< setup
            Integer(const Integer &);                 //!< duplicate
            Integer(const Natural &);                 //!< setup
            Integer(const SignType, const Natural &); //!< setup
            Integer & operator=(const Integer  &);    //!< assign
            Integer & operator=(const integer_t );    //!< assign
            Integer & operator=(const Natural  &);    //!< assign
            virtual ~Integer() noexcept;              //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Integer & xch( Integer & ) noexcept; //!< no-throw exchange

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()       const noexcept; //!< CallSign
            virtual size_t       serialize(OutputStream &) const; //!< mark+n
            static  Integer      Read(InputStream &);             //!< retriver serialized

            //__________________________________________________________________
            //
            //
            // comparison
            //
            //_________________________________________________________________
            friend bool operator==(const Integer & lhs, const Integer & rhs) noexcept; //!< equality
            friend bool operator==(const Integer & lhs, const integer_t rhs) noexcept; //!< equality
            friend bool operator==(const integer_t lhs, const Integer & rhs) noexcept; //!< equality
            friend bool operator==(const Integer & lhs, const Natural & rhs) noexcept; //!< equality
            friend bool operator==(const Natural & lhs, const Integer & rhs) noexcept; //!< equality

            friend bool operator!=(const Integer & lhs, const Integer & rhs) noexcept; //!< difference
            friend bool operator!=(const Integer & lhs, const integer_t rhs) noexcept; //!< difference
            friend bool operator!=(const integer_t lhs, const Integer & rhs) noexcept; //!< difference
            friend bool operator==(const Integer & lhs, const Natural & rhs) noexcept; //!< difference
            friend bool operator==(const Natural & lhs, const Integer & rhs) noexcept; //!< difference

            static SignType Compare(const Integer & lhs, const Integer & rhs) noexcept; //!< comparison
            static SignType Compare(const Integer & lhs, const integer_t rhs) noexcept; //!< comparison
            static SignType Compare(const integer_t lhs, const Integer & rhs) noexcept; //!< comparison

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

