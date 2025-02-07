//! \file

#ifndef Y_Apex_Rational_Included
#define Y_Apex_Rational_Included 1

#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {

        //! helper to implement comparisons
#define Y_Apex_Rational_Op(OP,LHS,RHS,MATCHES,RESULT)             \
/**/ inline friend                                                \
/**/ bool  operator OP (const LHS lhs, const RHS rhs)   {         \
/**/   return RESULT MATCHES Rational::Compare(lhs,rhs);          \
/**/ }

        //! helper to implement comparisons
#define Y_Apex_Rational_Ops(OP,MATCHES,RESULT)                  \
Y_Apex_Rational_Op(OP, Rational &, Rational &, MATCHES, RESULT) \
Y_Apex_Rational_Op(OP, Rational &, integer_t , MATCHES, RESULT) \
Y_Apex_Rational_Op(OP, integer_t , Rational &, MATCHES, RESULT) \
Y_Apex_Rational_Op(OP, Rational &, Integer & , MATCHES, RESULT) \
Y_Apex_Rational_Op(OP, Integer  &, Rational &, MATCHES, RESULT) \
Y_Apex_Rational_Op(OP, Rational &, Natural & , MATCHES, RESULT) \
Y_Apex_Rational_Op(OP, Natural  &, Rational &, MATCHES, RESULT)


        //______________________________________________________________________
        //
        //
        //
        //! Rational = Integer / Natural
        //
        //
        //______________________________________________________________________
        class Rational : public Number
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Apex::Rational"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Rational(const integer_t );                 //!< setup
            Rational(const Rational &);                 //!< setup
            Rational(const Natural  &);                 //!< setup
            Rational(const Integer  &);                 //!< setup
            Rational(const integer_t, const natural_t); //!< setup
            Rational(const Integer &, const Natural &); //!< setup
            Rational(Random::Bits &ran, const size_t nbits, const size_t dbits); //!< setup
            Rational(const String &);
            virtual ~Rational() noexcept; //!< cleanup


            Rational & operator=(const Rational &); //!< assign
            Rational & operator=(const integer_t ); //!< assign
            Rational & operator=(const String   &); //!< parse/assign
            Y_OSTREAM_PROTO(Rational);              //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Rational &      xch(Rational &q);      //!< no-throw exchange
            void            simplify();            //!< manual simplify
            static Rational Parse(const String &);

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()       const noexcept;  //!< CallSign
            virtual size_t       serialize(OutputStream &) const;  //!< numer+denom
            static  Rational     Read(InputStream &);              //!< reload serialized


            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________
            friend bool operator==(const Rational &, const Rational &) noexcept; //!< ==

            friend bool operator==(const Rational &, const integer_t ) noexcept; //!< ==
            friend bool operator==(const integer_t , const Rational &) noexcept; //!< ==
            friend bool operator==(const Rational &, const Integer  &) noexcept; //!< ==
            friend bool operator==(const Integer  &, const Rational &) noexcept; //!< ==

            friend bool operator!=(const Rational &, const Rational &) noexcept; //!< !=

            friend bool operator!=(const Rational &, const integer_t ) noexcept; //!< !=
            friend bool operator!=(const integer_t , const Rational &) noexcept; //!< !=
            friend bool operator!=(const Rational &, const Integer  &) noexcept; //!< !=
            friend bool operator!=(const Integer  &, const Rational &) noexcept; //!< !=

            static SignType Compare(const Rational &, const Rational &); //!< comparison
            static SignType Compare(const Rational &, const Integer  &); //!< comparison
            static SignType Compare(const Integer  &, const Rational &); //!< comparison
            static SignType Compare(const Rational &, const integer_t ); //!< comparison
            static SignType Compare(const integer_t,  const Rational &); //!< comparison

            static SignType Compare(const Rational &, const Natural  &); //!< comparison
            static SignType Compare(const Natural  &, const Rational &); //!< comparison

#ifndef DOXYGEN_SHOULD_SKIP_THIS
            Y_Apex_Rational_Ops(<, ==,Negative)
            Y_Apex_Rational_Ops(>, ==,Positive)
            Y_Apex_Rational_Ops(<=,!=,Positive)
            Y_Apex_Rational_Ops(>=,!=,Negative)
#endif

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Integer numer; //!< numerator
            const Natural denom; //!< denominator

        private:
            
        };
    }

}


#endif
