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
        //! Drop-in Signed Type replacement
        //
        //
        //______________________________________________________________________
        class Integer : public Number
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
            Integer();                             //!< zero
            virtual ~Integer() noexcept;           //!< cleanup

            Integer(const size_t, Random::Bits &);  //!< random

            Integer(const Integer &);              //!< copy   integer
            Integer & operator=(const Integer &);  //!< assign integer

            Integer(const Natural &);              //!< setup natural
            Integer & operator=(const Natural &);  //!< assign natural

            Integer(const int64_t);                //!< setup integral
            Integer & operator=(const int64_t);    //!< assign integral

            Integer(const SignType _s, const Natural _n); //!< build from sign+natural
            
            Y_OSTREAM_PROTO(Integer);              //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void xch(Integer &)     noexcept;  //!< no-throw exchange
            void appendTo(IO::Chars &) const;  //!< append to chars
            Integer incr() const;              //!< increase by 1
            Integer decr() const;              //!< decrease by 1

            //__________________________________________________________________
            //
            //
            // Addition
            //
            //__________________________________________________________________
            Integer operator+() const;                                        //!< unary plus
            friend Integer operator+(const Integer &lhs, const Integer &rhs); //!< lhs + rhs
            friend Integer operator+(const Integer &, const int64_t  );       //!< lhs + rhs
            friend Integer operator+(const int64_t  , const Integer &);       //!< lhs + rhs
            friend Integer operator+(const Integer &, const Natural &);       //!< lhs + rhs
            friend Integer operator+(const Natural &, const Integer &);       //!< lhs + rhs
            Integer & operator+=(const Integer &);                            //!< this += rhs
            Integer & operator+=(const int64_t  );                            //!< this += rhs
            Integer & operator+=(const Natural &);                            //!< this += rhs
            Integer & operator++();                                           //!< ++this
            Integer   operator++(int);                                        //!< this++

            //__________________________________________________________________
            //
            //
            // Subtraction
            //
            //__________________________________________________________________
            Integer operator-() const;                                         //!< unary minus
            friend Integer operator-(const Integer &lhs, const Integer &rhs);  //!< lhs + rhs
            friend Integer operator-(const Integer &, const int64_t  );        //!< lhs + rhs
            friend Integer operator-(const int64_t  , const Integer &);        //!< lhs + rhs
            friend Integer operator-(const Integer &, const Natural &);        //!< lhs + rhs
            friend Integer operator-(const Natural &, const Integer &);        //!< lhs + rhs
            Integer & operator-=(const Integer &);                             //!< this += rhs
            Integer & operator-=(const int64_t  );                             //!< this += rhs
            Integer & operator-=(const Natural &);                             //!< this += rhs
            Integer & operator--();                                            //!< --this
            Integer   operator--(int);                                         //!< this--

            //__________________________________________________________________
            //
            //
            // Multiplication
            //
            //__________________________________________________________________
            friend Integer operator*(const Integer &, const Integer &); //!< lhs * rhs
            friend Integer operator*(const Integer &, const int64_t  ); //!< lhs * rhs
            friend Integer operator*(const int64_t  , const Integer &); //!< lhs * rhs
            friend Integer operator*(const Integer &, const Natural &); //!< lhs * rhs
            friend Integer operator*(const Natural &, const Integer &); //!< lhs * rhs
            Integer & operator*=(const Integer &);                      //!< this *= rhs
            Integer & operator*=(const int64_t  );                      //!< this *= rhs
            Integer & operator*=(const Natural &);                      //!< this *= rhs

            //__________________________________________________________________
            //
            //
            // Division
            //
            //__________________________________________________________________
            friend Integer operator/(const Integer &, const Integer &); //!< lhs / rhs
            friend Integer operator/(const Integer &, const int64_t  ); //!< lhs / rhs
            friend Integer operator/(const int64_t  , const Integer &); //!< lhs / rhs
            friend Integer operator/(const Integer &, const Natural &); //!< lhs / rhs
            friend Integer operator/(const Natural &, const Integer &); //!< lhs / rhs
            Integer & operator/=(const Integer &);                      //!< this /= rhs
            Integer & operator/=(const int64_t  );                      //!< this /= rhs
            Integer & operator/=(const Natural &);                      //!< this /= rhs

            //__________________________________________________________________
            //
            //
            // comparison
            //
            //__________________________________________________________________

            //! class comparison
            static SignType Compare(const Integer &lhs, const Integer &rhs) noexcept;

            //! propagate 'friend bool operator OP (LHS,RHS) noexecept { return Cmp(LHS,RHS) RESULT; }'
#define Y_APZ_OP(OP,RESULT) \
inline friend bool operator OP (const Integer &lhs, const Integer &rhs ) noexcept { return Cmp(lhs,rhs) RESULT; }\
inline friend bool operator OP (const Integer &lhs, const int64_t  rhs ) noexcept { return Cmp(lhs,rhs) RESULT; }\
inline friend bool operator OP (const int64_t  lhs, const Integer &rhs ) noexcept { return Cmp(lhs,rhs) RESULT; }\
inline friend bool operator OP (const Integer &lhs, const Natural &rhs ) noexcept { return Cmp(lhs,rhs) RESULT; }\
inline friend bool operator OP (const Natural &lhs, const Integer &rhs ) noexcept { return Cmp(lhs,rhs) RESULT; }

            Y_APZ_OP(==, == __Zero__)
            Y_APZ_OP(!=, != __Zero__)
            Y_APZ_OP(<,  == Negative)
            Y_APZ_OP(>,  == Positive)
            Y_APZ_OP(<=, != Positive)
            Y_APZ_OP(>=, != Negative)

            static SignType Cmp(const Integer &lhs, const Integer &rhs) noexcept; //!< comparison helper

            static SignType Cmp(const Integer &lhs, const int64_t  rhs) noexcept; //!< comparison helper
            static SignType Cmp(const uint64_t lhs, const Integer &rhs) noexcept; //!< comparison helper

            static SignType Cmp(const Integer &lhs, const Natural &rhs) noexcept; //!< comparison helper
            static SignType Cmp(const Natural &lhs, const Integer &rhs) noexcept; //!< comparison helper

            //__________________________________________________________________
            //
            //
            // I/O
            //
            //__________________________________________________________________
            virtual size_t  serialize(OutputStream &) const; //!< s+n
            static  Integer ReadFrom(InputStream &);         //!< load serialized

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SignType s; //!< sign
            const Natural  n; //!< unsigned value



        };
    }

    typedef  Apex::Integer apz; //!< alias
}

#endif
