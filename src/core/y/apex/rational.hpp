
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

