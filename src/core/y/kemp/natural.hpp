//! \file

#ifndef Y_Kemp_Natural_Included
#define Y_Kemp_Natural_Included 1

#include "y/kemp/number.hpp"
#include "y/kemp/types.hpp"

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
            static Ops Strategy; //!< global strategy
            static const char * const CallSign;
            
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
            void xch(Natural &) noexcept; //!< noexcept exchange content
            String toHex()         const; //!< render to hexadecimal


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


        private:
            mutable Element *code;

            Natural(Element * const, const AsElement_ &) noexcept;
            void incr();                    //!< add 1
            void decr();                    //!< sub 1
            void make(Element *) noexcept;  //!< replace code

            static Natural Divide(const Natural &den, const Natural &num);
        };

    }
    typedef Kemp::Natural apn; //!< alias

}

#endif

