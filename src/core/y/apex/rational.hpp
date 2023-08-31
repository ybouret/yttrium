
//! \file

#ifndef Y_Apex_Rational_Included
#define Y_Apex_Rational_Included 1

#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! Rational (irreducible) number
        //
        //
        //______________________________________________________________________
        class Rational : public Number
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "apq"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Rational();                   //!< 0/1
            Rational(const Rational &);   //!< value/1
            Rational(const int64_t   );   //!< value/1
            Rational(const Integer  &);   //!< value/1
            Rational(const Natural  &);   //!< value/1
            virtual ~Rational() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Rational);    //!< dispplay

            Rational(const int64_t  , const uint64_t ); //!< frac
            Rational(const int64_t  , const Natural &); //!< frac
            Rational(const Integer &, const uint64_t ); //!< frac
            Rational(const Integer &, const Natural &); //!< frac

            Rational & operator=( const Rational &);    //!< assign
            Rational & operator=( const int64_t   );    //!< assign
            Rational & operator=( const Integer  &);    //!< assign
            Rational & operator=( const Natural  &);    //!< assign

            //! random
            Rational(const size_t numerBits,const size_t denomBits, Random::Bits &ran);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void xch(Rational &)    noexcept; //!< no-throw exchange
            void appendTo(IO::Chars &) const; //!< smart formatting
            void zset()             noexcept; //!< setting to zero, keeping memory, no exception

            //__________________________________________________________________
            //
            //
            // Equality
            //
            //__________________________________________________________________
            friend bool operator==(const Rational &lhs, const Rational &rhs) noexcept; //!< compare
            friend bool operator==(const Rational &lhs, const int64_t   rhs) noexcept; //!< compare
            friend bool operator==(const int64_t   lhs, const Rational &rhs) noexcept; //!< compare
            friend bool operator==(const Rational &lhs, const Integer  &rhs) noexcept; //!< compare
            friend bool operator==(const Integer  &lhs, const Rational &rhs) noexcept; //!< compare
            friend bool operator==(const Rational &lhs, const Natural  &rhs) noexcept; //!< compare
            friend bool operator==(const Natural  &lhs, const Rational &rhs) noexcept; //!< compare

            //__________________________________________________________________
            //
            //
            // Difference
            //
            //__________________________________________________________________
            friend bool operator!=(const Rational &lhs, const Rational &rhs) noexcept; //!< compare
            friend bool operator!=(const Rational &lhs, const int64_t   rhs) noexcept; //!< compare
            friend bool operator!=(const int64_t   lhs, const Rational &rhs) noexcept; //!< compare
            friend bool operator!=(const Rational &lhs, const Integer  &rhs) noexcept; //!< compare
            friend bool operator!=(const Integer  &lhs, const Rational &rhs) noexcept; //!< compare
            friend bool operator!=(const Rational &lhs, const Natural  &rhs) noexcept; //!< compare
            friend bool operator!=(const Natural  &lhs, const Rational &rhs) noexcept; //!< compare


            //__________________________________________________________________
            //
            //
            // helpers operators
            //
            //__________________________________________________________________
            static SignType Cmp(const Rational &lhs, const Rational &rhs); //!< compare

            static SignType Cmp(const Rational &lhs, const int64_t   rhs); //!< compare
            static SignType Cmp(const int64_t   lhs, const Rational &rhs); //!< compare

            static SignType Cmp(const Rational &lhs, const Integer  &rhs); //!< compare
            static SignType Cmp(const Integer  &lhs, const Rational &rhs); //!< compare

            static SignType Cmp(const Rational &lhs, const Natural  &rhs); //!< compare
            static SignType Cmp(const Natural  &lhs, const Rational &rhs); //!< compare

            //! propagate 'friend bool operator OP (LHS,RHS) noexecept { return Cmp(LHS,RHS) RESULT; }'
#define Y_APQ_OP(OP,RESULT) \
inline friend bool operator OP (const Rational &lhs, const Rational &rhs ) noexcept { return Rational::Cmp(lhs,rhs) RESULT; }\
inline friend bool operator OP (const Rational &lhs, const int64_t   rhs ) noexcept { return Rational::Cmp(lhs,rhs) RESULT; }\
inline friend bool operator OP (const int64_t   lhs, const Rational &rhs ) noexcept { return Rational::Cmp(lhs,rhs) RESULT; }\
inline friend bool operator OP (const Rational &lhs, const Integer  &rhs ) noexcept { return Rational::Cmp(lhs,rhs) RESULT; }\
inline friend bool operator OP (const Integer  &lhs, const Rational &rhs ) noexcept { return Rational::Cmp(lhs,rhs) RESULT; }\
inline friend bool operator OP (const Rational &lhs, const Natural  &rhs ) noexcept { return Rational::Cmp(lhs,rhs) RESULT; }\
inline friend bool operator OP (const Natural  &lhs, const Rational &rhs ) noexcept { return Rational::Cmp(lhs,rhs) RESULT; }

            Y_APQ_OP(<,  == Negative)
            Y_APQ_OP(>,  == Positive)
            Y_APQ_OP(<=, != Positive)
            Y_APQ_OP(>=, != Negative)


            //__________________________________________________________________
            //
            //
            // elementary operators
            //
            //__________________________________________________________________

            //! declare all possible operators
#define Y_APQ_BINARY_DECL(OP)                                            \
/**/  friend Rational   operator OP (const Rational &, const Rational &);\
/**/  friend Rational   operator OP (const Rational &, const int64_t   );\
/**/  friend Rational   operator OP (const int64_t   , const Rational &);\
/**/  friend Rational   operator OP (const Integer  &, const Rational &);\
/**/  friend Rational   operator OP (const Rational &, const Integer  &);\
/**/  friend Rational   operator OP (const Natural  &, const Rational &);\
/**/  friend Rational   operator OP (const Rational &, const Natural  &);\
/**/         Rational & operator OP##=(const Rational &);                \
/**/         Rational & operator OP##=(const int64_t   );                \
/**/         Rational & operator OP##=(const Integer  &);                \
/**/         Rational & operator OP##=(const Natural  &)


            //! body of in-place operator
#define Y_APQ_BINARY_BODY(OP) { { Rational tmp = (*this) OP lhs; xch(tmp); } return *this; }

            //! implement all in-place operators
#define Y_APQ_BINARY_IMPL(OP) \
Rational & Rational:: operator OP##=(const Rational & lhs) Y_APQ_BINARY_BODY(OP)\
Rational & Rational:: operator OP##=(const int64_t    lhs) Y_APQ_BINARY_BODY(OP)\
Rational & Rational:: operator OP##=(const Integer  & lhs) Y_APQ_BINARY_BODY(OP)\
Rational & Rational:: operator OP##=(const Natural  & lhs) Y_APQ_BINARY_BODY(OP)


            //__________________________________________________________________
            //
            //
            // Add
            //
            //__________________________________________________________________
            Rational operator+() const; //!< unary +

            //! all operators
            Y_APQ_BINARY_DECL(+);


            //__________________________________________________________________
            //
            //
            // Sub
            //
            //__________________________________________________________________
            Rational operator-() const; //!< unary -
            //! all operators
            Y_APQ_BINARY_DECL(-);


            //__________________________________________________________________
            //
            //
            // Mul
            //
            //__________________________________________________________________

            //! all operators
            Y_APQ_BINARY_DECL(*);

            //__________________________________________________________________
            //
            //
            // Div
            //
            //__________________________________________________________________

            //! all operators
            Y_APQ_BINARY_DECL(/);


            //__________________________________________________________________
            //
            //
            // Other
            //
            //__________________________________________________________________
            static Rational Abs(const Rational &); //!< absolute value
            Rational        abs()           const; //!< absolute value
            static Rational Sqrt(const Rational &); //!< absolute value
            Rational        sqrt()           const; //!< absolute value

            
            //__________________________________________________________________
            //
            //
            // I/O
            //
            //__________________________________________________________________
            virtual size_t   serialize(OutputStream &) const; //!< numer+denom
            static  Rational ReadFrom(InputStream &);         //!< load serialized

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Integer numer; //!< numerator
            const Natural denom; //!< denominator

        private:
            void zDenom() const;
            void update();

        };


    }

    typedef Apex::Rational apq; //!< alias
}
#endif

