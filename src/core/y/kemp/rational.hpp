
//! \file

#ifndef Y_Kemp_Rational_Included
#define Y_Kemp_Rational_Included 1

#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Kemp
    {

#define Y_Rational_Copy_and_Assign(TYPE) \
inline Rational(const TYPE rhs) : Number(), numer(rhs), denom(1) {} \
inline Rational & operator=(const TYPE rhs) { Rational _(rhs); xch(_); return *this; }\

#define Y_Rational_Ctor(LHS,RHS) \
inline Rational(const LHS num, const RHS den) : Number(), numer(num), denom(den) { validate(); }

        class Rational : public Number
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "apq"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Rational() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Rational);

            Rational();                   //! 0/1
            Rational(const size_t, const size_t, Random::Bits &ran);
            Rational(const Rational &);
            Rational & operator=(const Rational &);

            Y_Rational_Copy_and_Assign(Integer&)
            Y_Rational_Copy_and_Assign(Natural&)
            Y_Rational_Copy_and_Assign(int64_t)

            Y_Rational_Ctor(Integer &, Natural &)
            Y_Rational_Ctor(Natural &, Natural &)
            Y_Rational_Ctor(int64_t,   Natural &)
            Y_Rational_Ctor(Integer &, uint64_t)
            Y_Rational_Ctor(Natural &, uint64_t)
            Y_Rational_Ctor(int64_t,   uint64_t)

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t       serialize(OutputStream &) const;
            virtual const char * callSign()       const noexcept;


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            String toString() const;
            void   validate();
            void   xch(Rational &q) noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Integer numer; //!< numerator
            const Natural denom; //!< denominator
        };

    }

    typedef Kemp::Rational apq;

}

#endif

