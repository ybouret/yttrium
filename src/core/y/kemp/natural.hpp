//! \file

#ifndef Y_Kemp_Natural_Included
#define Y_Kemp_Natural_Included 1

#include "y/kemp/number.hpp"
#include "y/kemp/types.hpp"
#include "y/type/ints.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        class Element;
        Y_SHALLOW_DECL(AsElement); //!< alias for internal constructor


        //______________________________________________________________________
        //
        //
        //! helper to duplicate arguments with same code
        //
        //______________________________________________________________________
#define Y_Kemp_Natural_Binary_NoExcept(RET,FUNC,CODE) \
/**/ RET FUNC(const Natural &lhs, const Natural &rhs) noexcept { CODE; } \
/**/ RET FUNC(const uint64_t lhs, const Natural &rhs) noexcept { CODE; } \
/**/ RET FUNC(const Natural &lhs, const uint64_t rhs) noexcept { CODE; }

        //______________________________________________________________________
        //
        //
        //! generate comparators with various arguments
        //
        //______________________________________________________________________
#define Y_Kemp_Natural_Cmp(OP,EXPR) \
Y_Kemp_Natural_Binary_NoExcept(friend inline bool,OP,return Compare(lhs,rhs) EXPR)


        //______________________________________________________________________
        //
        //
        //! helper to duplicate arguments with same code
        //
        //______________________________________________________________________
#define Y_Kemp_Natural_Binary_Decl(FUNC)                          \
/**/ friend Natural FUNC(const Natural &lhs, const Natural &rhs); \
/**/ friend Natural FUNC(const uint64_t lhs, const Natural &rhs); \
/**/ friend Natural FUNC(const Natural &lhs, const uint64_t rhs)


        //______________________________________________________________________
        //
        //
        //
        //! Natural, unsing internal Element
        //
        //
        //______________________________________________________________________
        class Natural : public Number
        {
        public:
            //__________________________________________________________________
            //
            //
            // definitions
            //
            //__________________________________________________________________
            static Ops                Strategy; //!< global strategy
            static const char * const CallSign; //!< "apn"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Y_OSTREAM_PROTO(Natural);    //!< display decimal/hexadecimal
            virtual ~Natural() noexcept; //!< cleanup

            Natural();                //!< default setup = 0
            Natural(const Natural &); //!< copy
            Natural(const uint64_t);  //!< set to qword

            Natural(const size_t nbits, Random::Bits &);   //!< exactly nbits random bits
            Natural(const TwoToThe_ &, const size_t ibit); //!< 2^ibit

            Natural &operator=(const Natural &); //!< assign
            Natural &operator=(const uint64_t);  //!< assign qword

            Element & operator*() const noexcept; //!< get internal element

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void   xch(Natural &)                  noexcept; //!< noexcept exchange content
            String toHex()                            const; //!< render to hexadecimal
            String toDec()                            const; //!< render to decimal
            size_t bits()                    const noexcept; //!< get current bits
            size_t size()                    const noexcept; //!< get current bytes
            uint8_t operator[](const size_t) const noexcept; //!< access in 0..size()-1

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________
            static SignType Compare(const Natural &lhs, const Natural &rhs) noexcept; //!< comparison
            static SignType Compare(const uint64_t lhs, const Natural &rhs) noexcept; //!< comparison
            static SignType Compare(const Natural &lhs, const uint64_t rhs) noexcept; //!< comparison


            //! aliases
            Y_Kemp_Natural_Cmp(operator==, ==__Zero__)
            Y_Kemp_Natural_Cmp(operator<,  ==Negative)
            Y_Kemp_Natural_Cmp(operator<=, !=Positive)
            Y_Kemp_Natural_Cmp(operator>,  ==Positive)
            Y_Kemp_Natural_Cmp(operator>=, !=Negative)

            //__________________________________________________________________
            //
            //
            // Bits
            //
            //__________________________________________________________________
            Natural & shr() noexcept;                                 //!< in place 1 bit right shift: /=2
            Natural & shr(const size_t);                              //!< right shift
            Natural & shl(const size_t);                              //!< left shift
            Natural & operator<<=(const size_t);                      //!< left shift
            Natural & operator>>=(const size_t);                      //!< righ shift
            friend Natural operator<<(const Natural &, const size_t); //!< left shift
            friend Natural operator>>(const Natural &, const size_t); //!< right shift

            //__________________________________________________________________
            //
            //
            // Additions
            //
            //__________________________________________________________________
            Natural   operator+() const;            //!< unary +
            Natural & operator+=(const Natural &);  //!< in place +
            Natural & operator+=(const uint64_t );  //!< in place +
            Natural & operator++();                 //!< pre-increment
            Natural   operator++(int);              //!< post-increment
            Y_Kemp_Natural_Binary_Decl(operator+);  //!< aliases

            //__________________________________________________________________
            //
            //
            // Subtractions
            //
            //__________________________________________________________________
            Natural & operator-=(const Natural &);  //!< in place -
            Natural & operator-=(const uint64_t );  //!< in place -
            Natural & operator--();                 //!< pre-decrement
            Natural   operator--(int);              //!< post-decrement
            Y_Kemp_Natural_Binary_Decl(operator-);  //!< aliases

            //__________________________________________________________________
            //
            //
            // Multiplications
            //
            //__________________________________________________________________
            Natural & operator*=(const Natural &);  //!< in place *
            Natural & operator*=(const uint64_t );  //!< in place *
            Y_Kemp_Natural_Binary_Decl(operator*);  //!< aliases

            //__________________________________________________________________
            //
            //
            // Divisions
            //
            //__________________________________________________________________
            Natural & operator/=(const Natural &);  //!< in place /
            Natural & operator/=(const uint64_t );  //!< in place /
            Y_Kemp_Natural_Binary_Decl(operator/);  //!< aliases

            //! num = q * den + r
            static void Div(Natural &q, Natural &r, const Natural &num, const Natural &den);

            //__________________________________________________________________
            //
            //
            // Modulos
            //
            //__________________________________________________________________
            Natural & operator%=(const Natural &);  //!< in place /
            Natural & operator%=(const uint64_t );  //!< in place /
            Y_Kemp_Natural_Binary_Decl(operator%);  //!< aliases


            //__________________________________________________________________
            //
            //
            // Conversion
            //
            //__________________________________________________________________

            //! try cast to integral T
            template <typename T> inline
            bool tryCast(T &target) const noexcept
            {               
                static const size_t         Size  = sizeof(T);
                typedef UnsignedInt<Size>   Call;
                typedef typename Call::Type WORD;
                static const size_t         Bits  = IsSigned<T>::Value ? (8*Size-1) : 8*Size;
                if(bits()>Bits)
                    return false;
                else
                {
                    WORD u = 0;
                    for(size_t i=size();i>0;)
                        u = Call::SHL8(u) | (*this)[--i];
                    target = static_cast<T>(u);
                    return true;
                }
            }

            //! cast with exception raisin
            template <typename T> inline
            T cast(const char *ctx=0) const
            {
                T target = 0;
                if(!tryCast(target)) CastOverflow(ctx);
                return target;
            }


        private:
            mutable Element *code;

            Natural(Element * const, const AsElement_ &) noexcept;
            void incr();                    //!< add 1
            void decr();                    //!< sub 1
            void make(Element *) noexcept;  //!< replace code

            static Natural Divide(const Natural &den, const Natural &num);
            static Natural Modulo(const Natural &den, const Natural &num);

            static void CastOverflow(const char *ctx);


        };

    }
    typedef Kemp::Natural apn; //!< alias

}

#endif

