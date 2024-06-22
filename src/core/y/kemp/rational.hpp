
//! \file

#ifndef Y_Kemp_Rational_Included
#define Y_Kemp_Rational_Included 1

#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        //______________________________________________________________________
        //
        //! helper for various arguments
        //______________________________________________________________________
#define Y_Rational_Copy_and_Assign(TYPE) \
inline Rational(const TYPE rhs) : Number(), numer(rhs), denom(1) {} \
inline Rational & operator=(const TYPE rhs) { Rational _(rhs); xch(_); return *this; }\

        //______________________________________________________________________
        //
        //! helper for various arguments
        //______________________________________________________________________

#define Y_Rational_Ctor(LHS,RHS) \
inline Rational(const LHS num, const RHS den) : Number(), numer(num), denom(den) { validate(); }

        //______________________________________________________________________
        //
        //! helper for various arguments
        //______________________________________________________________________
#define Y_Kemp_Rational_Operator(OP,CALL) \
inline Rational & operator OP##= ( const Rational &rhs ) { Rational _ = CALL(*this,rhs); xch(_); return *this; } \
inline Rational & operator OP##= ( const Integer  &rhs ) { Rational _ = CALL(*this,rhs); xch(_); return *this; } \
inline Rational & operator OP##= ( const Natural  &rhs ) { Rational _ = CALL(*this,rhs); xch(_); return *this; } \
inline Rational & operator OP##= ( const int64_t   rhs ) { Rational _ = CALL(*this,rhs); xch(_); return *this; } \
inline friend Rational operator OP (const Rational &lhs, const Rational &rhs) { return CALL(lhs,rhs); }          \
inline friend Rational operator OP (const Rational &lhs, const Integer  &rhs) { return CALL(lhs,rhs); }          \
inline friend Rational operator OP (const Rational &lhs, const Natural  &rhs) { return CALL(lhs,rhs); }          \
inline friend Rational operator OP (const Rational &lhs, const int64_t   rhs) { return CALL(lhs,rhs); }          \
inline friend Rational operator OP (const Integer  &lhs, const Rational &rhs) { return CALL(lhs,rhs); }          \
inline friend Rational operator OP (const Natural  &lhs, const Rational &rhs) { return CALL(lhs,rhs); }          \
inline friend Rational operator OP (const int64_t  &lhs, const Rational &rhs) { return CALL(lhs,rhs); }

        //______________________________________________________________________
        //
        //! helper for various arguments
        //______________________________________________________________________
#define Y_Kemp_Rational_Call(RETURN,FUNC)                     \
static RETURN FUNC(const Rational &lhs, const Rational &rhs); \
static RETURN FUNC(const Rational &lhs, const Integer  &rhs); \
static RETURN FUNC(const Rational &lhs, const Natural  &rhs); \
static RETURN FUNC(const Rational &lhs, const int64_t   rhs); \
static RETURN FUNC(const Integer  &lhs, const Rational &rhs); \
static RETURN FUNC(const Natural  &lhs, const Rational &rhs); \
static RETURN FUNC(const int64_t   lhs, const Rational &rhs)

        //______________________________________________________________________
        //
        //! implement function with various arguments
        //______________________________________________________________________
#define Y_Kemp_Rational_API(FUNC) Y_Kemp_Rational_Call(Rational,FUNC)

        //______________________________________________________________________
        //
        //! implement comparison
        //______________________________________________________________________
#define Y_Kemp_Rational_Cmp(OP,LTYPE,RTYPE,EXPR) \
inline friend bool operator OP (const LTYPE lhs, const RTYPE rhs) { return Compare(lhs,rhs) EXPR; }

        //______________________________________________________________________
        //
        //! implement comparisons
        //______________________________________________________________________
#define Y_Kemp_Rational_Compare(OP,EXPR)           \
Y_Kemp_Rational_Cmp(OP, Rational&, Rational&,EXPR) \
Y_Kemp_Rational_Cmp(OP, Rational&, Integer &,EXPR) \
Y_Kemp_Rational_Cmp(OP, Rational&, Natural &,EXPR) \
Y_Kemp_Rational_Cmp(OP, Rational&, int64_t  ,EXPR) \
Y_Kemp_Rational_Cmp(OP, Integer &, Rational&,EXPR) \
Y_Kemp_Rational_Cmp(OP, Natural &, Rational&,EXPR) \
Y_Kemp_Rational_Cmp(OP, int64_t  , Rational&,EXPR)



        //______________________________________________________________________
        //
        //
        //
        //! Rational = Integer/Natural
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
            static const char * const CallSign; //!< "apq"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Rational() noexcept;                            //!< cleanup
            Y_OSTREAM_PROTO(Rational);                               //!< display
            Rational(const size_t, const size_t, Random::Bits &ran); //!< random
            Rational();                                              //!< 0/1
            Rational(const Rational &);                              //!< copy
            Rational & operator=(const Rational &);                  //!< assign
            

#ifndef DOXYGEN_SHOULD_SKIP_THIS
            Y_Rational_Copy_and_Assign(Integer&)
            Y_Rational_Copy_and_Assign(Natural&)
            Y_Rational_Copy_and_Assign(int64_t)
            Y_Rational_Ctor(Integer &, Natural &)
            Y_Rational_Ctor(Natural &, Natural &)
            Y_Rational_Ctor(int64_t,   Natural &)
            Y_Rational_Ctor(Integer &, uint64_t)
            Y_Rational_Ctor(Natural &, uint64_t)
            Y_Rational_Ctor(int64_t,   uint64_t)
#endif

            Rational(const SignType, const Natural &, const Natural &); //!< developped constructor

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t       serialize(OutputStream &) const;
            static  Rational     ReadFrom(InputStream &); //!< read from stream
            virtual const char * callSign()       const noexcept;


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            String     toString() const;          //!< convert to string
            void       validate();                //!< simplify, check no zero denom
            void       xch(Rational &q) noexcept; //!< no-throw exchange
            Rational & neg()            noexcept; //!< change sign
            Rational & ldz()            noexcept; //!< set 0
            Rational & ld1()            noexcept; //!< set 1

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________
            Y_Kemp_Rational_Call(SignType,Compare); //!< aliases

            //__________________________________________________________________
            //
            //
            // Operations
            //
            //__________________________________________________________________
#ifndef DOXYGEN_SHOULD_SKIP_THIS
            Y_Kemp_Rational_Compare(==,  == __Zero__)
            Y_Kemp_Rational_Compare(!=,  != __Zero__)
            Y_Kemp_Rational_Compare(<,   == Negative)
            Y_Kemp_Rational_Compare(>,   == Positive)
            Y_Kemp_Rational_Compare(<=,  != Positive)
            Y_Kemp_Rational_Compare(>=,  != Negative)
            Y_Kemp_Rational_Operator(*,Mul)
            Y_Kemp_Rational_Operator(/,Div)
            Y_Kemp_Rational_Operator(+,Add)
            Y_Kemp_Rational_Operator(-,Sub)
#endif

            Rational   operator+() const; //!< unary plus
            void       incr();            //!< +1
            Rational & operator++();      //!< pre-increment
            Rational   operator++(int);   //!< post-increment

            Rational  operator-() const; //!< unary minus
            void       decr();           //!< -1
            Rational & operator--();     //!< pre-decrement
            Rational   operator--(int);  //!< post-decrement

            //__________________________________________________________________
            //
            //
            // Other methods
            //
            //__________________________________________________________________
            static Rational Sqr(const Rational &q);  //!< |q|^2
            Rational        sqr()            const;  //!< |*this|^2
            static Rational Abs(const Rational &q);  //!< |q|
            Rational        abs()            const;  //!< |*this|
            static Rational Sqrt(const Rational &q); //!< sqrt(q)
            Rational        sqrt()            const; //!< sqrt(*this)

            template <typename T>
            T toReal() const; //!< floating point approximation [XReal](float|double|long double)


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Integer numer; //!< numerator
            const Natural denom; //!< denominator

        private:
            static void     DivisionByZero();
            Y_Kemp_Rational_API(Mul);
            Y_Kemp_Rational_API(Div);
            Y_Kemp_Rational_API(Add);
            Y_Kemp_Rational_API(Sub);
        };

    }

    typedef Kemp::Rational apq;

}

#endif

