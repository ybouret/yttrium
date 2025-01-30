
//! \file

#ifndef Y_Apex_Integer_Included
#define Y_Apex_Integer_Included 1

#include "y/apex/natural.hpp"

namespace Yttrium
{

    namespace Apex
    {


        //! helper to implement comparisons
#define Y_Apex_Integer_Op(OP,LHS,RHS,MATCHES,RESULT)             \
/**/ inline friend                                               \
/**/ bool  operator OP (const LHS lhs, const RHS rhs) noexcept { \
/**/   return RESULT MATCHES Integer::Compare(lhs,rhs);          \
/**/ }

        //! helper to implement comparisons
#define Y_Apex_Integer_Ops(OP,MATCHES,RESULT)               \
Y_Apex_Integer_Op(OP,Integer &, Integer &, MATCHES, RESULT) \
Y_Apex_Integer_Op(OP,Integer &, integer_t, MATCHES, RESULT) \
Y_Apex_Integer_Op(OP,integer_t, Integer &, MATCHES, RESULT) \
Y_Apex_Integer_Op(OP,Integer &, Natural &, MATCHES, RESULT) \
Y_Apex_Integer_Op(OP,Natural &, Integer &, MATCHES, RESULT)

        //! helper to declare binary functions
#define Y_Apex_Integer_Binary_Wrapper(FCN) \
static Integer FCN(const Integer & lhs, const Integer & rhs);\
static Integer FCN(const Integer & lhs, const integer_t rhs);\
static Integer FCN(const integer_t lhs, const Integer & rhs);\
static Integer FCN(const Integer & lhs, const Natural & rhs);\
static Integer FCN(const Natural & lhs, const Integer & rhs)

        //! implement operator from a binary function
#define Y_Apex_Integer_Binary_Operator(OP,LHS,RHS,FCN) \
friend inline Integer operator OP (const LHS lhs, const RHS rhs) { return Integer:: FCN(lhs,rhs); }

        //! implement operators from a binary function
#define Y_Apex_Integer_Binary_Operators(OP,FCN)               \
Y_Apex_Integer_Binary_Operator(OP, Integer &, Integer &, FCN) \
Y_Apex_Integer_Binary_Operator(OP, Integer &, integer_t, FCN) \
Y_Apex_Integer_Binary_Operator(OP, integer_t, Integer &, FCN) \
Y_Apex_Integer_Binary_Operator(OP, Integer &, Natural &, FCN) \
Y_Apex_Integer_Binary_Operator(OP, Natural &, Integer &, FCN)

        //! implement unary operator from a binary function
#define Y_Apex_Integer_Unary_Operator(OP,RHS,FCN) \
inline Integer & operator OP##= (const RHS rhs) { Integer _( FCN(*this,rhs) ); xch(_); return *this; }

        //! implement unary operators from a binary function
#define Y_Apex_Integer_Unary_Operators(OP,FCN)   \
Y_Apex_Integer_Unary_Operator(OP,Integer &,FCN)  \
Y_Apex_Integer_Unary_Operator(OP,integer_t,FCN)  \
Y_Apex_Integer_Unary_Operator(OP,Natural &,FCN)

        //! implement all operators fron a binary function
#define Y_Apex_Integer_Operators(OP,FCN) \
Y_Apex_Integer_Binary_Operators(OP,FCN)  \
Y_Apex_Integer_Unary_Operators(OP,FCN)

        //______________________________________________________________________
        //
        //
        //
        //! Integer = sign + natural
        //
        //
        //______________________________________________________________________
        class Integer : public Number
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const char * const CallSign;        //!< Apex::Integert
            static const uint8_t Is__Zero__ = 0x00;    //!< alias
            static const uint8_t IsPositive = 0x01;    //!< alias
            static const uint8_t IsNegative = 0x02;    //!< alias



            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Integer(const integer_t z);               //!< setup
            Integer(const Integer &);                 //!< duplicate
            Integer(const Natural &);                 //!< setup
            Integer(const SignType, const Natural &); //!< setup
            Integer & operator=(const Integer  &);    //!< assign
            Integer & operator=(const integer_t );    //!< assign
            Integer & operator=(const Natural  &);    //!< assign
            virtual ~Integer() noexcept;              //!< cleanup
            Integer(Random::Bits &, const size_t);    //!< with random sign
            Y_OSTREAM_PROTO(Integer);                 //!< output as decimal string

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Integer & xch( Integer & ) noexcept; //!< no-throw exchange
            String    toDec() const;             //!< to decimal string

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()       const noexcept; //!< CallSign
            virtual size_t       serialize(OutputStream &) const; //!< mark+n
            static  Integer      Read(InputStream &);             //!< retriver serialized

            //__________________________________________________________________
            //
            //
            // comparisons
            //
            //__________________________________________________________________
            friend bool operator==(const Integer & lhs, const Integer & rhs) noexcept; //!< equality
            friend bool operator==(const Integer & lhs, const integer_t rhs) noexcept; //!< equality
            friend bool operator==(const integer_t lhs, const Integer & rhs) noexcept; //!< equality
            friend bool operator==(const Integer & lhs, const Natural & rhs) noexcept; //!< equality
            friend bool operator==(const Natural & lhs, const Integer & rhs) noexcept; //!< equality

            friend bool operator!=(const Integer & lhs, const Integer & rhs) noexcept; //!< difference
            friend bool operator!=(const Integer & lhs, const integer_t rhs) noexcept; //!< difference
            friend bool operator!=(const integer_t lhs, const Integer & rhs) noexcept; //!< difference
            friend bool operator==(const Integer & lhs, const Natural & rhs) noexcept; //!< difference
            friend bool operator==(const Natural & lhs, const Integer & rhs) noexcept; //!< difference

            static SignType Compare(const Integer & lhs, const Integer & rhs) noexcept; //!< comparison
            static SignType Compare(const Integer & lhs, const integer_t rhs) noexcept; //!< comparison
            static SignType Compare(const integer_t lhs, const Integer & rhs) noexcept; //!< comparison
            static SignType Compare(const Integer & lhs, const Natural & rhs) noexcept; //!< comparison
            static SignType Compare(const Natural & lhs, const Integer & rhs) noexcept; //!< comparison


#ifndef DOXYGEN_SHOULD_SKIP_THIS
            Y_Apex_Integer_Ops(<, ==,Negative)
            Y_Apex_Integer_Ops(>, ==,Positive)
            Y_Apex_Integer_Ops(<=,!=,Positive)
            Y_Apex_Integer_Ops(>=,!=,Negative)
#endif

            //__________________________________________________________________
            //
            //
            // Additions
            //
            //__________________________________________________________________
            Integer operator+() const;          //!< unary +
            Integer &      operator ++ ();      //!< ++this
            Integer        operator ++ (int);   //!< this++
            void           incr();              //!< +=1
            
            //__________________________________________________________________
            //
            //
            // Subtractions
            //
            //__________________________________________________________________
            Integer        operator-() const; //!< unary -
            Integer &      operator -- ();    //!< --this
            Integer        operator -- (int); //!< this--
            void           decr();            //!< -=1

            //__________________________________________________________________
            //
            //
            // Operators
            //
            //__________________________________________________________________
#ifndef DOXYGEN_SHOULD_SKIP_THIS
            Y_Apex_Integer_Operators(+,Add)
            Y_Apex_Integer_Operators(-,Sub)
            Y_Apex_Integer_Operators(*,Mul)
#endif

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SignType s; //!< sign
            const Natural  n; //!< synchronized natural

        private:
            Y_Apex_Integer_Binary_Wrapper(Add);
            Y_Apex_Integer_Binary_Wrapper(Sub);
            Y_Apex_Integer_Binary_Wrapper(Mul);

        };

    }

}

#endif

