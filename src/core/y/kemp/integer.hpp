
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
            // C++
            //
            //__________________________________________________________________
            Integer();                                //!< zero
            Integer(const int64_t);                   //!< integer
            Integer(const Integer &);                 //!< copy
            Integer(const Natural &);                 //!< copy
            Integer(const SignType, const Natural &); //!< copy/chk
            Integer(const SignType, const uint64_t);  //!< copy/chk
            virtual ~Integer() noexcept;              //!< cleanup
            Integer & operator=(const Integer &z);    //!< assign
            Integer & operator=(const int64_t  z);    //!< assign
            Integer & operator=(const Natural  &);    //!< assign
            Y_OSTREAM_PROTO(Integer);                 //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void chk()           noexcept; //!< sign in sync with absolute value
            void xch(Integer &z) noexcept; //!< safe exchange

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________

            //! comparison
            static inline
            SignType Compare(const Integer &lhs, const Integer &rhs) noexcept
            {
                return Cmp(lhs.s,lhs.n,rhs.s,rhs.n);
            }

            //! comparison
            static inline
            SignType Compare(const Integer &lhs, const Natural &rhs) noexcept
            {
                return Cmp(lhs.s,lhs.n,rhs.sign(),rhs);
            }

            //! comparison
            static inline
            SignType Compare(const Natural &lhs, const Integer &rhs) noexcept
            {
                return Cmp(lhs.sign(),lhs,rhs.s,rhs.n);
            }

            //! comparison
            static inline
            SignType Compare(const Integer &lhs, const int64_t rhs) noexcept
            {
                static const uint64_t zero = 0;
                switch( Sign::Of(rhs) )
                {
                    case __Zero__: break;
                    case Negative: { const uint64_t u = static_cast<uint64_t>(-rhs); return Cmp(lhs.s,lhs.n,Negative,u); }
                    case Positive: { const uint64_t u = static_cast<uint64_t>( rhs); return Cmp(lhs.s,lhs.n,Positive,u); }
                }
                return Cmp(lhs.s,lhs.n,__Zero__,zero);
            }

            //! comparison
            static inline
            SignType Compare(const int64_t lhs, const Integer &rhs) noexcept
            {
                static const uint64_t zero = 0;
                switch( Sign::Of(lhs) )
                {
                    case __Zero__: break;
                    case Negative: { const uint64_t u = static_cast<uint64_t>(-lhs); return Cmp(Negative,u,rhs.s,rhs.n); }
                    case Positive: { const uint64_t u = static_cast<uint64_t>( lhs); return Cmp(Positive,u,rhs.s,rhs.n); }
                }
                return Cmp(__Zero__,zero,rhs.s,rhs.n);
            }

            //__________________________________________________________________
            //
            //
            // Additions
            //
            //__________________________________________________________________

            //! unary +
            inline Integer operator+() const { return *this; }

            //! in-place +
            inline Integer & operator+=( const Integer &rhs ) {
                Integer _ = Add(*this,rhs); xch(_); return *this;
            }

            //! in place +
            inline Integer & operator+=( const Natural &rhs ) {
                Integer _ = Add(*this,rhs); xch(_); return *this;
            }

            //! in place +
            inline Integer & operator+=( const int64_t rhs ) {
                Integer _ = Add(*this,rhs); xch(_); return *this;
            }

            //! +
            inline friend Integer operator+(const Integer &lhs, const Integer &rhs) {
                return Add(lhs,rhs);
            }

            //! +
            inline friend Integer operator+(const Integer &lhs, const Natural &rhs) {
                return Add(lhs,rhs);
            }

            //! +
            inline friend Integer operator+(const Natural &lhs, const Integer &rhs) {
                return Add(lhs,rhs);
            }


            //! +
            inline friend Integer operator+(const Integer &lhs, const int64_t rhs) {
                return Add(lhs,rhs);
            }

            //! +
            inline friend Integer operator+(const int64_t lhs, const Integer &rhs) {
                return Add(lhs,rhs);
            }

            //__________________________________________________________________
            //
            //
            // Subtractions
            //
            //__________________________________________________________________

            //! unary minus
            inline Integer operator-() const { return Integer( Sign::Opposite(s), n ); }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SignType s; //!< sign
            const Natural  n; //!< absolute value
                              //!
        private:

            //! generic comparison
            /**
             \param ls lhs sign
             \param ln rhs natural (Natural|uint64_t)
             \param rs rhs sign
             \param rn rhs natural (Natural|uint64_t)
             */
            template <typename LHS_UNSIGNED, typename RHS_UNSIGNED> static inline
            SignType Cmp(const SignType      ls,
                         const LHS_UNSIGNED &ln,
                         const SignType      rs,
                         const RHS_UNSIGNED &rn) noexcept
            {
                switch( Sign::MakePair(ls,rs) )
                {
                        // special case
                    case ZZ_Signs: assert(0==ln); assert(0==rn); break;

                        // trivial POSITIVE signs ls > rs
                    case PZ_Signs: return Positive;
                    case PN_Signs: return Positive;
                    case ZN_Signs: return Positive;


                        // trivial NEGATIVE signs ls < rs
                    case ZP_Signs: return Negative;
                    case NP_Signs: return Negative;
                    case NZ_Signs: return Negative;

                        // all positive signs
                    case PP_Signs: return Natural::Compare(ln,rn);

                        // all negative signs
                    case NN_Signs: return Natural::Compare(rn,ln);
                }

                return __Zero__;
            }


            //! generic addition
            /**
             \param ls lhs sign
             \param ln rhs natural (Natural|uint64_t)
             \param rs rhs sign
             \param rn rhs natural (Natural|uint64_t)
             */
            template <typename LHS_UNSIGNED, typename RHS_UNSIGNED> static inline
            Integer Add(const SignType      ls,
                        const LHS_UNSIGNED &ln,
                        const SignType      rs,
                        const RHS_UNSIGNED &rn)
            {
                switch( Sign::MakePair(ls,rs) )
                {
                        // special case
                    case ZZ_Signs: assert(0==ln); assert(0==rn); break;

                        // special case with one ZERO
                    case NZ_Signs: return Integer(ls,ln);
                    case PZ_Signs: return Integer(ls,ln);
                    case ZN_Signs: return Integer(rs,rn);
                    case ZP_Signs: return Integer(rs,rn);

                        // all POSITIVE:
                    case PP_Signs: { const Natural sum = ln+rn; return Integer(Positive,sum); }

                        // all NEGATIVE:
                    case NN_Signs: { const Natural sum = ln+rn; return Integer(Negative,sum); }

                        // mixed case
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

                        // mixed case
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
            static Integer Add(const Integer &lhs, const uint64_t rhs);
            static Integer Add(const uint64_t lhs, const Integer &rhs);



        };

    }

    typedef Kemp::Integer apz;

}

#endif

