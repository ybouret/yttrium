
//! \file

#ifndef Y_Kemp_Integer_Included
#define Y_Kemp_Integer_Included 1

#include "y/kemp/natural.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        //______________________________________________________________________
        //
        //
        //! helper to implement operators from the same CALL function
        //
        //______________________________________________________________________
#define Y_Kemp_Integer_Operator(OP,CALL) \
inline Integer & operator OP##= ( const Integer &rhs ) { Integer _ = CALL(*this,rhs); xch(_); return *this; } \
inline Integer & operator OP##= ( const Natural &rhs ) { Integer _ = CALL(*this,rhs); xch(_); return *this; } \
inline Integer & operator OP##= ( const int64_t  rhs ) { Integer _ = CALL(*this,rhs); xch(_); return *this; } \
inline friend Integer operator OP (const Integer &lhs, const Integer &rhs) { return CALL(lhs,rhs); } \
inline friend Integer operator OP (const Integer &lhs, const Natural &rhs) { return CALL(lhs,rhs); } \
inline friend Integer operator OP (const Natural &lhs, const Integer &rhs) { return CALL(lhs,rhs); } \
inline friend Integer operator OP (const Integer &lhs, const int64_t  rhs) { return CALL(lhs,rhs); } \
inline friend Integer operator OP (const int64_t  lhs, const Integer &rhs) { return CALL(lhs,rhs); } \

        //______________________________________________________________________
        //
        //! helper to implement a comparison function
        //______________________________________________________________________
#define Y_Kemp_Integer_Cmp(OP,LTYPE,RTYPE,RESULT) \
inline friend bool operator OP (const LTYPE lhs, const RTYPE rhs) noexcept { return Compare(lhs,rhs) RESULT; }

        //______________________________________________________________________
        //
        //! helper to implement all comparisons
        //______________________________________________________________________
#define Y_Kemp_Integer_Compare(OP,RESULT)       \
Y_Kemp_Integer_Cmp(OP,Integer&,Integer&,RESULT) \
Y_Kemp_Integer_Cmp(OP,Integer&,Natural&,RESULT) \
Y_Kemp_Integer_Cmp(OP,Natural&,Integer&,RESULT) \
Y_Kemp_Integer_Cmp(OP,Integer&,int64_t, RESULT) \
Y_Kemp_Integer_Cmp(OP,int64_t, Integer&,RESULT)

        //______________________________________________________________________
        //
        //! helper to implement all functions
        //______________________________________________________________________
#define Y_Kemp_Integer_API(FUNC) \
static Integer FUNC(const Integer &lhs, const Integer &rhs); \
static Integer FUNC(const Integer &lhs, const Natural &rhs); \
static Integer FUNC(const Natural &lhs, const Integer &rhs); \
static Integer FUNC(const Integer &lhs, const int64_t  rhs); \
static Integer FUNC(const int64_t  lhs, const Integer &rhs)

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
            static const char * const CallSign; //!< "apz"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Integer();                                //!< zero
            Integer(const int64_t);                   //!< integer
            Integer(const Integer &);                 //!< copy
            Integer(const Natural &);                 //!< copy
            Integer(const SignType, const Natural &); //!< copy/chk
            Integer(const SignType, const uint64_t);  //!< copy/chk
            Integer(const size_t, Random::Bits&);     //!< random
            Integer(const String &);                  //!< [+/-] decimal
            virtual ~Integer() noexcept;              //!< cleanup
            Integer & operator=(const Integer &z);    //!< assign
            Integer & operator=(const int64_t  z);    //!< assign
            Integer & operator=(const Natural  &);    //!< assign
            Y_OSTREAM_PROTO(Integer);                 //!< display

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t       serialize(OutputStream &) const;
            static  Integer      ReadFrom(InputStream &);         //!< read from stream
            virtual const char * callSign()       const noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void     chk()            noexcept; //!< sign in sync with absolute value
            void     xch(Integer &z)  noexcept; //!< safe exchange
            String   toString()          const; //!< make string
            Integer &neg()            noexcept; //!< change sign

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________
            static SignType Compare(const Integer &lhs, const Integer &rhs) noexcept; //!< comparison
            static SignType Compare(const Integer &lhs, const Natural &rhs) noexcept; //!< comparison
            static SignType Compare(const Natural &lhs, const Integer &rhs) noexcept; //!< comparison
            static SignType Compare(const Integer &lhs, const int64_t  rhs) noexcept; //!< comparison
            static SignType Compare(const int64_t  lhs, const Integer &rhs) noexcept; //!< comparison

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

#ifndef DOXYGEN_SHOULD_SKIP_THIS
            Y_Kemp_Integer_Compare(==, == __Zero__)
            Y_Kemp_Integer_Compare(!=, != __Zero__)
            Y_Kemp_Integer_Compare(<,  == Negative)
            Y_Kemp_Integer_Compare(>,  == Positive)
            Y_Kemp_Integer_Compare(<=, != Positive)
            Y_Kemp_Integer_Compare(>=, != Negative)
            Y_Kemp_Integer_Operator(+,Add)
            Y_Kemp_Integer_Operator(-,Sub)
            Y_Kemp_Integer_Operator(*,Mul)
            Y_Kemp_Integer_Operator(/,Div)
#endif

            Integer   operator+() const; //!< unary plus
            Integer & operator++();      //!< pre-increment
            Integer   operator++(int);   //!< post-increment
            Integer   operator-() const; //!< unary minus
            Integer & operator--();      //!< pre-decrement
            Integer   operator--(int);   //!< post-decrement

            //__________________________________________________________________
            //
            //
            // Other
            //
            //__________________________________________________________________
            static Integer Sqr(const Integer &z); //!< z^2
            Integer        sqr()           const; //!< *this^2
            static Integer Abs(const Integer &z); //!< |n|
            Integer        abs()           const; //!< |*this|
            static Integer Sqrt(const Integer &); //!< integer square root
            Integer        sqrt()          const; //!< integer square root

            //__________________________________________________________________
            //
            //
            // Conversion
            //
            //__________________________________________________________________

            //! try to cast to integer value
            template <typename T> inline
            bool tryCast(T &target) const noexcept
            {
                static const Int2Type< IsSigned<T>::Value > choice = {};
                return tryCast(target,choice);
            }

            //__________________________________________________________________
            //
            //! cast with exception raising on overflow
            //__________________________________________________________________
            template <typename T> inline
            T cast(const char *ctx=0) const
            {
                T target = 0;
                if(!tryCast(target)) CastOverflow(ctx);
                return target;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SignType s; //!< sign
            const Natural  n; //!< absolute value

        private:
            void        incr();           //!< add 1
            void        decr();           //!< sub 1
            static void DivisionByZero(); //!< raise exception
            static void CastOverflow(const char *);

#include "integer/cmp.hpp"
#include "integer/add.hpp"
#include "integer/mul.hpp"
#include "integer/div.hpp"
            Y_Kemp_Integer_API(Add);
            Y_Kemp_Integer_API(Sub);
            Y_Kemp_Integer_API(Mul);
            Y_Kemp_Integer_API(Div);

            //! unsigned conversion
            template <typename T> inline
            bool tryCast(T &target, const Int2Type<false> &) const noexcept
            {
                static const size_t MaxBits = sizeof(T) * 8;

                switch(s)
                {
                    case Negative:           return false;
                    case __Zero__: target=0; return true;
                    case Positive:
                        break;
                }
                if(n.bits()>MaxBits) return false;
                target = n.buildWord<T>();
                return true;
            }

            //! signed conversion
            template <typename T> inline
            bool tryCast(T &target, const Int2Type<true> &) const noexcept
            {
                static const size_t MaxBits = sizeof(T) * 8 - 1;

                switch(s)
                {
                    case __Zero__: break;
                    case Positive: 
                        if(n.bits()>MaxBits) return false;
                        target = n.buildWord<T>();
                        return true;

                    case Negative:
                        if(n.bits()>MaxBits) return false;
                        target = -n.buildWord<T>();
                        return true;
                }
                
                target = 0;
                return true;
            }

        };

    }

    typedef Kemp::Integer apz;

}

#endif

