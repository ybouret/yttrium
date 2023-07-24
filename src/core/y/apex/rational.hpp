
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

            Rational & operator=( const Rational &); //!< assign
            Rational & operator=( const int64_t   ); //!< assign
            Rational & operator=( const Integer  &); //!< assign
            Rational & operator=( const Natural  &); //!< assign

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void xch(Rational &)    noexcept; //!< no-throw exchange
            void appendTo(IO::Chars &) const; //!< smart formatting

            //__________________________________________________________________
            //
            //
            // Comparison
            //
            //__________________________________________________________________

            friend bool operator==(const Rational &lhs, const Rational &rhs) noexcept; //!< compare
            friend bool operator==(const Rational &lhs, const int64_t   rhs) noexcept; //!< compare
            friend bool operator==(const int64_t   lhs, const Rational &rhs) noexcept; //!< compare
            friend bool operator==(const Rational &lhs, const Integer  &rhs) noexcept; //!< compare
            friend bool operator==(const Integer  &lhs, const Rational &rhs) noexcept; //!< compare




            //! declare all possible operators
#define Y_APQ_BINARY_DECL(OP) \
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
            Y_APQ_BINARY_DECL(+);       //!< all operators


            //__________________________________________________________________
            //
            //
            // Sub
            //
            //__________________________________________________________________
            Rational operator-() const; //!< unary -
            Y_APQ_BINARY_DECL(-);       //!< all operators

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

