//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/number.hpp"
#include "y/ostream-proto.hpp"
#include "y/config/shallow.hpp"
#include "y/type/signs.hpp"

namespace Yttrium
{
    namespace Random { class Bits; }

    //! helper
    Y_SHALLOW_DECL(TwoToThe);
    
    namespace Apex
    {

        //! helper
        Y_SHALLOW_DECL(AsImpl);



        //______________________________________________________________________
        //
        //
        //
        //! Drop-in Unsigned Type replacement
        //
        //
        //______________________________________________________________________
        class Natural : public Number
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Natural();                                   //!< default=0
            Natural(const uint64_t);                     //!< default from uint
            Natural(const Natural &);                    //!< copy
            Natural(const size_t n, Random::Bits &);     //!< random with exactly n bits
            Natural &operator=(const Natural &);         //!< assign another
            Natural &operator=(const uint64_t) noexcept; //!< assign an uint
            virtual ~Natural() noexcept;                 //!< cleannup
            Y_OSTREAM_PROTO(Natural);                    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            size_t         bytes() const noexcept;         //!< exact number of bytes
            size_t         bits()  const noexcept;         //!< exact number of bit
            void           xch(Natural &) noexcept;        //!< no-throw exchange
            std::ostream & printHex(std::ostream &) const; //!< compact print hexa
            uint64_t       u64() const noexcept;           //!< get least significant bits
            //__________________________________________________________________
            //
            //
            // ADD
            //
            //__________________________________________________________________
            friend Natural operator+(const Natural &lhs, const Natural &rhs); //!< lhs+rhs
            friend Natural operator+(const Natural &lhs, const uint64_t rhs); //!< lhs+rhs
            friend Natural operator+(const uint64_t lhs, const Natural &rhs); //!< lhs+rhs
            Natural & operator+=(const Natural &rhs);                         //!< this += rhs
            Natural & operator+=(const uint64_t rhs);                         //!< this += rhs
            Natural & operator++();                                           //!< ++this
            Natural   operator++(int);                                        //!< this++

            //__________________________________________________________________
            //
            //
            // SUB
            //
            //__________________________________________________________________
            friend Natural operator-(const Natural &lhs, const Natural &rhs); //!< lhs-rhs
            friend Natural operator-(const Natural &lhs, const uint64_t rhs); //!< lhs-rhs
            friend Natural operator-(const uint64_t lhs, const Natural &rhs); //!< lhs-rhs
            Natural & operator-=(const Natural &rhs);                         //!< this -= rhs
            Natural & operator-=(const uint64_t rhs);                         //!< this -= rhs
            Natural & operator--();                                           //!< --this
            Natural   operator--(int);                                        //!< this--

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________
            static SignType Compare(const Natural &lhs, const Natural &rhs) noexcept; //!< increasing order

            friend bool operator==(const Natural &lhs, const Natural &rhs) noexcept; //!< lhs == rhs
            friend bool operator==(const Natural &lhs, const uint64_t rhs) noexcept; //!< lhs == rhs
            friend bool operator==(const uint64_t lhs, const Natural &rhs) noexcept; //!< lhs == rhs

            friend bool operator!=(const Natural &lhs, const Natural &rhs) noexcept; //!< lhs != rhs
            friend bool operator!=(const Natural &lhs, const uint64_t rhs) noexcept; //!< lhs != rhs
            friend bool operator!=(const uint64_t lhs, const Natural &rhs) noexcept; //!< lhs != rhs

            friend bool operator<(const Natural &lhs, const Natural &rhs) noexcept; //!< lhs < rhs
            friend bool operator<(const Natural &lhs, const uint64_t rhs) noexcept; //!< lhs < rhs
            friend bool operator<(const uint64_t lhs, const Natural &rhs) noexcept; //!< lhs < rhs

            friend bool operator>(const Natural &lhs, const Natural &rhs) noexcept; //!< lhs > rhs
            friend bool operator>(const Natural &lhs, const uint64_t rhs) noexcept; //!< lhs > rhs
            friend bool operator>(const uint64_t lhs, const Natural &rhs) noexcept; //!< lhs > rhs


            friend bool operator<=(const Natural &lhs, const Natural &rhs) noexcept; //!< lhs < rhs
            friend bool operator<=(const Natural &lhs, const uint64_t rhs) noexcept; //!< lhs < rhs
            friend bool operator<=(const uint64_t lhs, const Natural &rhs) noexcept; //!< lhs < rhs


            friend bool operator>=(const Natural &lhs, const Natural &rhs) noexcept; //!< lhs > rhs
            friend bool operator>=(const Natural &lhs, const uint64_t rhs) noexcept; //!< lhs > rhs
            friend bool operator>=(const uint64_t lhs, const Natural &rhs) noexcept; //!< lhs > rhs


            //__________________________________________________________________
            //
            //
            // Bitwise ops
            //
            //__________________________________________________________________
            explicit Natural(const TwoToThe_ &, const size_t p); //!< 2^p

            //__________________________________________________________________
            //
            //
            // Multiplication
            //
            //__________________________________________________________________
            friend Natural operator*(const Natural &lhs, const Natural &rhs); //!< lhs*rhs
            friend Natural operator*(const Natural &lhs, const uint64_t rhs); //!< lhs*rhs
            friend Natural operator*(const uint64_t lhs, const Natural &rhs); //!< lhs*rhs
            Natural & operator*=(const Natural &rhs);                         //!< this * rhs
            Natural & operator*=(const uint64_t rhs);                         //!< this * rhs

        private:
            void *impl;
            Natural(void*,const AsImpl_&) noexcept;
            
        };

    }

    typedef Apex::Natural apn; //!< alias

}

#endif

