
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
            virtual const char * callSign()       const noexcept;
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void   chk()            noexcept; //!< sign in sync with absolute value
            void   xch(Integer &z)  noexcept; //!< safe exchange
            String toString()          const; //!< make string

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
            // Comparisons
            //
            //__________________________________________________________________

#ifndef DOXYGEN_SHOULD_SKIP_THIS
            Y_Kemp_Integer_Compare(==, == __Zero__)
            Y_Kemp_Integer_Compare(!=, != __Zero__)
            Y_Kemp_Integer_Compare(<,  == Negative)
            Y_Kemp_Integer_Compare(>,  == Positive)
            Y_Kemp_Integer_Compare(<=, != Positive)
            Y_Kemp_Integer_Compare(>=, != Negative)
#endif


            //__________________________________________________________________
            //
            //
            // Additions
            //
            //__________________________________________________________________

            //! unary +
            inline Integer operator+() const { return *this; }
            Y_Kemp_Integer_Operator(+,Add)
            Integer & operator++();                 //!< pre-increment
            Integer   operator++(int);              //!< post-increment

            static Integer Square(const Integer &);
            Integer        square() const;


            //__________________________________________________________________
            //
            //
            // Subtractions
            //
            //__________________________________________________________________

            //! unary minus
            inline Integer operator-() const { return Integer( Sign::Opposite(s), n ); }
            Y_Kemp_Integer_Operator(-,Sub)
            Integer & operator--();                 //!< pre-decrement
            Integer   operator--(int);              //!< post-decrement
                                                    //!
            //__________________________________________________________________
            //
            //
            // Multiplication
            //
            //__________________________________________________________________

            Y_Kemp_Integer_Operator(*,Mul)


            //__________________________________________________________________
            //
            //
            // Division
            //
            //__________________________________________________________________

            Y_Kemp_Integer_Operator(/,Div)


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SignType s; //!< sign
            const Natural  n; //!< absolute value

        private:

            void incr();                    //!< add 1
            void decr();                    //!< sub 1
                                            
            //__________________________________________________________________
            //
            //! generic comparison with NO EXCEPTION
            /**
             \param ls lhs sign
             \param ln rhs natural (Natural|uint64_t)
             \param rs rhs sign
             \param rn rhs natural (Natural|uint64_t)
             */
            //__________________________________________________________________
            template <typename LHS_UNSIGNED, typename RHS_UNSIGNED> static inline
            SignType Cmp(const SignType      ls,
                         const LHS_UNSIGNED &ln,
                         const SignType      rs,
                         const RHS_UNSIGNED &rn) noexcept
            {
                switch( Sign::MakePair(ls,rs) )
                {
                        //------------------------------------------------------
                        // special case
                        //------------------------------------------------------
                    case ZZ_Signs: assert(0==ln); assert(0==rn); break;

                        //------------------------------------------------------
                        // trivial POSITIVE signs ls > rs
                        //------------------------------------------------------
                    case PZ_Signs: return Positive;
                    case PN_Signs: return Positive;
                    case ZN_Signs: return Positive;

                        //------------------------------------------------------
                        // trivial NEGATIVE signs ls < rs
                        //------------------------------------------------------
                    case ZP_Signs: return Negative;
                    case NP_Signs: return Negative;
                    case NZ_Signs: return Negative;

                        //------------------------------------------------------
                        // all positive signs
                        //------------------------------------------------------
                    case PP_Signs: return Natural::Compare(ln,rn);

                        //------------------------------------------------------
                        // all negative signs
                        //------------------------------------------------------
                    case NN_Signs: return Natural::Compare(rn,ln);
                }

                return __Zero__;
            }


            //__________________________________________________________________
            //
            //! generic addition
            /**
             \param ls lhs sign
             \param ln rhs natural (Natural|uint64_t)
             \param rs rhs sign
             \param rn rhs natural (Natural|uint64_t)
             */
            //__________________________________________________________________
            template <typename LHS_UNSIGNED, typename RHS_UNSIGNED> static inline
            Integer Add(const SignType      ls,
                        const LHS_UNSIGNED &ln,
                        const SignType      rs,
                        const RHS_UNSIGNED &rn)
            {
                switch( Sign::MakePair(ls,rs) )
                {
                        //------------------------------------------------------
                        // special case
                        //------------------------------------------------------
                    case ZZ_Signs: assert(0==ln); assert(0==rn); break;

                        //------------------------------------------------------
                        // special case with one ZERO
                        //------------------------------------------------------
                    case NZ_Signs: return Integer(ls,ln);
                    case PZ_Signs: return Integer(ls,ln);
                    case ZN_Signs: return Integer(rs,rn);
                    case ZP_Signs: return Integer(rs,rn);

                        //------------------------------------------------------
                        // all POSITIVE:
                        //------------------------------------------------------
                    case PP_Signs: { const Natural sum = ln+rn; return Integer(Positive,sum); }

                        //------------------------------------------------------
                        // all NEGATIVE:
                        //------------------------------------------------------
                    case NN_Signs: { const Natural sum = ln+rn; return Integer(Negative,sum); }

                        //------------------------------------------------------
                        // mixed case
                        //------------------------------------------------------
                    case PN_Signs:  {
                        assert(ln>0); assert(rn>0);
                        assert(ls==Positive);
                        assert(rs==Negative);
                        switch( Natural::Compare(ln,rn) )
                        {
                            case Positive: { const Natural dif = ln-rn; return Integer(Positive,dif); }
                            case __Zero__: break;
                            case Negative: { const Natural dif = rn-ln; return Integer(Negative,dif); }
                        }
                    } return Integer();

                        //------------------------------------------------------
                        // mixed case
                        //------------------------------------------------------
                    case NP_Signs: {
                        assert(ln>0); assert(rn>0);
                        assert(ls==Negative);
                        assert(rs==Positive);
                        switch( Natural::Compare(ln,rn) )
                        {
                            case Positive: { const Natural dif = ln-rn; return Integer(Negative,dif); }
                            case __Zero__: break;
                            case Negative: { const Natural dif = rn-ln; return Integer(Positive,dif); }
                        }
                    } return Integer();

                }
                return Integer();
            }


            static Integer Add(const Integer &lhs, const Integer &rhs);
            static Integer Add(const Integer &lhs, const Natural &rhs);
            static Integer Add(const Natural &lhs, const Integer &rhs);
            static Integer Add(const Integer &lhs, const int64_t  rhs);
            static Integer Add(const int64_t  lhs, const Integer &rhs);

            static Integer Sub(const Integer &lhs, const Integer &rhs);
            static Integer Sub(const Integer &lhs, const Natural &rhs);
            static Integer Sub(const Natural &lhs, const Integer &rhs);
            static Integer Sub(const Integer &lhs, const int64_t  rhs);
            static Integer Sub(const int64_t  lhs, const Integer &rhs);


            //__________________________________________________________________
            //
            //! generic multiplication
            /**
             \param ls lhs sign
             \param ln rhs natural (Natural|uint64_t)
             \param rs rhs sign
             \param rn rhs natural (Natural|uint64_t)
             */
            //__________________________________________________________________
            template <typename LHS_UNSIGNED, typename RHS_UNSIGNED> static inline
            Integer Mul(const SignType      ls,
                        const LHS_UNSIGNED &ln,
                        const SignType      rs,
                        const RHS_UNSIGNED &rn)
            {
                switch( Sign::MakePair(ls,rs) )
                {
                        //------------------------------------------------------
                        // trivial __Zero__
                        //------------------------------------------------------
                    case ZZ_Signs:
                    case ZN_Signs:
                    case ZP_Signs:
                    case NZ_Signs:
                    case PZ_Signs:
                        break;

                        //------------------------------------------------------
                        // Positive Result
                        //------------------------------------------------------
                    case PP_Signs:
                    case NN_Signs: { const Natural p = ln * rn; return Integer(Positive,p); }

                        //------------------------------------------------------
                        // Negative Result
                        //------------------------------------------------------
                    case PN_Signs:
                    case NP_Signs: { const Natural p = ln * rn; return Integer(Negative,p); }

                }
                return Integer();
            }

            static Integer Mul(const Integer &lhs, const Integer &rhs);
            static Integer Mul(const Integer &lhs, const Natural &rhs);
            static Integer Mul(const Natural &lhs, const Integer &rhs);
            static Integer Mul(const Integer &lhs, const int64_t  rhs);
            static Integer Mul(const int64_t  lhs, const Integer &rhs);


            static void DivisionByZero();
            //__________________________________________________________________
            //
            //! generic division
            /**
             \param ls lhs sign
             \param ln rhs natural (Natural|uint64_t)
             \param rs rhs sign
             \param rn rhs natural (Natural|uint64_t)
             */
            //__________________________________________________________________
            template <typename LHS_UNSIGNED, typename RHS_UNSIGNED> static inline
            Integer Div(const SignType      ls,
                        const LHS_UNSIGNED &ln,
                        const SignType      rs,
                        const RHS_UNSIGNED &rn)
            {
                switch( Sign::MakePair(ls,rs) )
                {
                        //------------------------------------------------------
                        // zero denominator
                        //------------------------------------------------------
                    case ZZ_Signs:
                    case NZ_Signs:
                    case PZ_Signs:
                        DivisionByZero();
                        break;

                        //------------------------------------------------------
                        // zero result
                        //------------------------------------------------------
                    case ZN_Signs:
                    case ZP_Signs:
                        break;

                        //------------------------------------------------------
                        // Positive result
                        //------------------------------------------------------
                    case NN_Signs:
                    case PP_Signs: {
                        const Natural q = ln/rn;
                        return Integer(Positive,q);
                    }

                        //------------------------------------------------------
                        // Negative result
                        //------------------------------------------------------
                    case NP_Signs:
                    case PN_Signs: {
                        const Natural q = ln/rn;
                        return Integer(Negative,q);
                    }
                }
                return Integer();
            }

            static Integer Div(const Integer &lhs, const Integer &rhs);
            static Integer Div(const Integer &lhs, const Natural &rhs);
            static Integer Div(const Natural &lhs, const Integer &rhs);
            static Integer Div(const Integer &lhs, const int64_t  rhs);
            static Integer Div(const int64_t  lhs, const Integer &rhs);

        };

    }

    typedef Kemp::Integer apz;

}

#endif

