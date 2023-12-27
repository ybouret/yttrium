//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/number.hpp"
#include "y/ostream-proto.hpp"
#include "y/config/shallow.hpp"
#include "y/type/signs.hpp"
#include "y/type/ints.hpp"

namespace Yttrium
{
    class InputStream;
    namespace Random { class Bits;  }
    Y_SHALLOW_DECL(TwoToThe); //!< helper
    
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
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Apex::Natural"

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
            void           appendHex( IO::Chars & ) const; //!< convert to hexadecimal chars
            void           appendDec( IO::Chars & ) const; //!< convert to decimal chars
            size_t         bytes()         const noexcept; //!< exact number of bytes
            size_t         bits()          const noexcept; //!< exact number of bit
            void           xch(Natural &)        noexcept; //!< no-throw exchange
            uint64_t       u64()           const noexcept; //!< get least significant bits
            SignType       getSign()       const noexcept; //!< __Zero__ or Positive
            void           zset()                noexcept; //!< setting to zero, keeping memory, no exception

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
            Natural   operator+() const;                                      //!< unary +
            
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
            Natural   operator-() const;                                      //!< unary -, only if zero!

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
            void shr()          noexcept; //!< >>= 1, in place, no-throw
            bool isEven() const noexcept; //!< check
            bool isOdd()  const noexcept; //!< check

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

            //__________________________________________________________________
            //
            //
            // Division
            //
            //__________________________________________________________________
            friend Natural operator/(const Natural &lhs, const Natural &rhs); //!< lhs*rhs
            friend Natural operator/(const Natural &lhs, const uint64_t rhs); //!< lhs/rhs
            friend Natural operator/(const uint64_t lhs, const Natural &rhs); //!< lhs/rhs
            Natural & operator/=(const Natural &rhs);                         //!< this / rhs
            Natural & operator/=(const uint64_t rhs);                         //!< this / rhs


            //__________________________________________________________________
            //
            //
            // Modulo
            //
            //__________________________________________________________________
            friend Natural operator%(const Natural &lhs, const Natural &rhs); //!< lhs%rhs
            friend Natural operator%(const Natural &lhs, const uint64_t rhs); //!< lhs%rhs
            friend Natural operator%(const uint64_t lhs, const Natural &rhs); //!< lhs%rhs
            Natural & operator%=(const Natural &rhs);                         //!< this % rhs
            Natural & operator%=(const uint64_t rhs);                         //!< this % rhs

            //! to be optimized
            static void Div(Natural &quot, Natural &rem, const Natural &numer, const Natural &denom)
            {
                quot = numer/denom; rem = numer - quot * denom;
            }

            //__________________________________________________________________
            //
            //
            // Arithmetic
            //
            //__________________________________________________________________
            static Natural GCD(const Natural &, const Natural &);    //!< Greatest Common Divider
            static Natural LCM(const Natural &, const Natural &);    //!< Least Common Multiplier, a>0, b>0
            static void    Simplify(Natural &Numer, Natural &Denom); //!< Simplify
            static Natural Factorial(size_t n);                      //!< factorial
            static Natural Comb(const size_t n, const size_t k);     //!< (n,k)
            static Natural Sqrt(const Natural &s);                   //!< square root
            Natural        sqrt() const;                             //!< square root

            template <typename T> static
            T ToReal(const Natural &numer, const Natural &denom);

            //__________________________________________________________________
            //
            //
            // conversion
            //
            //__________________________________________________________________

            //! no-throw try to cast to an integer type
            template <typename T> inline
            bool tryCast(T &target) const noexcept
            {
                static const size_t maxBits = sizeof(T)*8 - (IsSigned<T>::Value ? 1 : 0);
                if(bits()>maxBits) return false;
                target = static_cast<T>( u64() );
                return true;
            }

            //! cast to an integer type, throw on overflow
            template <typename T> inline
            T cast(const char *which) const
            {
                T target(0);
                if(!tryCast(target)) ThrowOverflow(which);
                return target;
            }

            //__________________________________________________________________
            //
            //
            // I/O
            //
            //__________________________________________________________________
            virtual size_t  serialize(OutputStream &) const; //!< size+data
            static  Natural ReadFrom(InputStream&);          //!< load serialized

        private:
            void *impl;
            Natural(void*,const AsImpl_&) noexcept;
            static void ThrowOverflow(const char *which);
        };

    }

    typedef Apex::Natural apn; //!< alias

}

#endif

