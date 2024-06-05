
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

#define Y_Kemp_Rational_Operator(OP,CALL) \
inline Rational & operator OP##= ( const Rational &rhs ) { Rational _ = CALL(*this,rhs); xch(_); return *this; } \
inline Rational & operator OP##= ( const Integer  &rhs ) { Rational _ = CALL(*this,rhs); xch(_); return *this; } \
inline Rational & operator OP##= ( const Natural  &rhs ) { Rational _ = CALL(*this,rhs); xch(_); return *this; } \
inline Rational & operator OP##= ( const int64_t   rhs ) { Rational _ = CALL(*this,rhs); xch(_); return *this; } \
inline friend Rational operator OP (const Rational &lhs, const Rational &rhs) { return CALL(lhs,rhs); } \
inline friend Rational operator OP (const Rational &lhs, const Integer  &rhs) { return CALL(lhs,rhs); } \
inline friend Rational operator OP (const Rational &lhs, const Natural  &rhs) { return CALL(lhs,rhs); } \
inline friend Rational operator OP (const Rational &lhs, const int64_t   rhs) { return CALL(lhs,rhs); } \
inline friend Rational operator OP (const Integer  &lhs, const Rational &rhs) { return CALL(lhs,rhs); } \
inline friend Rational operator OP (const Natural  &lhs, const Rational &rhs) { return CALL(lhs,rhs); } \
inline friend Rational operator OP (const int64_t  &lhs, const Rational &rhs) { return CALL(lhs,rhs); } 


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

            Rational(const SignType, const Natural &, const Natural &);

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
            // Multiplications
            //
            //__________________________________________________________________
            Y_Kemp_Rational_Operator(*,Mul)

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Integer numer; //!< numerator
            const Natural denom; //!< denominator

        private:
            static Rational Mul(const Rational &lhs, const Rational &rhs);
            static Rational Mul(const Rational &lhs, const Integer &rhs);
            static Rational Mul(const Rational &lhs, const Natural &rhs);
            static Rational Mul(const Rational &lhs, const int64_t  rhs);
            static Rational Mul(const Integer  &lhs, const Rational &rhs);
            static Rational Mul(const Natural  &lhs, const Rational &rhs);
            static Rational Mul(const int64_t   lhs, const Rational &rhs);


        };

    }

    typedef Kemp::Rational apq;

}

#endif

