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

        //! helper to declare binary functions
#define Y_Apex_Rational_Binary_Wrapper(FCN) \
static Rational FCN(const Rational &, const Rational &); \
static Rational FCN(const Rational &, const Integer  &); \
static Rational FCN(const Integer  &, const Rational &); \
static Rational FCN(const Rational &, const integer_t ); \
static Rational FCN(const integer_t,  const Rational &); \
static Rational FCN(const Rational &, const Natural  &); \
static Rational FCN(const Natural  &, const Rational &)

        //! implement operator from a binary function
#define Y_Apex_Rational_Binary_Operator(OP,LHS,RHS,FCN) \
friend inline Rational operator OP (const LHS lhs, const RHS rhs) { return Rational:: FCN(lhs,rhs); }

        //! implement operators from a binary function
#define Y_Apex_Rational_Binary_Operators(OP,FCN)                 \
Y_Apex_Rational_Binary_Operator(OP, Rational &, Rational &, FCN) \
Y_Apex_Rational_Binary_Operator(OP, Rational &, Integer  &, FCN) \
Y_Apex_Rational_Binary_Operator(OP, Integer  &, Rational &, FCN) \
Y_Apex_Rational_Binary_Operator(OP, Rational &, integer_t,  FCN) \
Y_Apex_Rational_Binary_Operator(OP, integer_t,  Rational &, FCN) \
Y_Apex_Rational_Binary_Operator(OP, Rational &, Natural  &, FCN) \
Y_Apex_Rational_Binary_Operator(OP, Natural  &, Rational &, FCN)

        //! implement unary operator from a binary function
#define Y_Apex_Rational_Unary_Operator(OP,RHS,FCN) \
inline Rational & operator OP##= (const RHS rhs) { Rational _( FCN(*this,rhs) ); xch(_); return *this; }

        //! implement unary operators from a binary function
#define Y_Apex_Rational_Unary_Operators(OP,FCN)     \
Y_Apex_Rational_Unary_Operator(OP, Rational &, FCN)   \
Y_Apex_Rational_Unary_Operator(OP, Integer  &, FCN)   \
Y_Apex_Rational_Unary_Operator(OP, integer_t , FCN)   \
Y_Apex_Rational_Unary_Operator(OP, Natural  &, FCN)

        //! implement all operators from a binary function
#define Y_Apex_Rational_Operators(OP,FCN) \
Y_Apex_Rational_Binary_Operators(OP,FCN)  \
Y_Apex_Rational_Unary_Operators(OP,FCN)

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
            Rational(const Natural &, const Natural &); //!< setup
            Rational(Random::Bits &ran, const size_t nbits, const size_t dbits); //!< setup
            Rational(const String &);                   //!< parse from string
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
            static Rational Parse(const String &); //!< parse from string

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
            //! cast to real
            //
            //__________________________________________________________________
            template <typename T> inline
            T cast() const
            {
                switch(numer.s)
                {
                    case __Zero__: break;
                    case Positive: return  Natural::Ratio<T>(numer.n,denom);
                    case Negative: return -Natural::Ratio<T>(numer.n,denom);
                }
                return 0;
            }

            //__________________________________________________________________
            //
            //
            // Additions
            //
            //__________________________________________________________________
            Rational operator+() const; //!< unary +
            void incr();                //!< +=1
            Rational &      operator ++ ();                     //!< ++this
            Rational        operator ++ (int);                  //!< this++

            //__________________________________________________________________
            //
            //
            // Subtractions
            //
            //__________________________________________________________________
            Rational operator-() const;     //!< unary -
            void decr();                    //!< +=1
            Rational &      operator -- ();       //!< --this
            Rational        operator -- (int);    //!< this--


            //__________________________________________________________________
            //
            //
            // Operators
            //
            //__________________________________________________________________
#ifndef DOXYGEN_SHOULD_SKIP_THIS
            Y_Apex_Rational_Operators(+,Add)
            Y_Apex_Rational_Operators(-,Sub)
            Y_Apex_Rational_Operators(*,Mul)
            Y_Apex_Rational_Operators(/,Div)
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
            Y_Apex_Rational_Binary_Wrapper(Add);
            Y_Apex_Rational_Binary_Wrapper(Sub);
            Y_Apex_Rational_Binary_Wrapper(Mul);
            Y_Apex_Rational_Binary_Wrapper(Div);

        };
    }

}


#endif
