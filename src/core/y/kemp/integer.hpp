
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
            static inline 
            SignType Compare(const Integer &lhs, const Integer &rhs) noexcept
            {
                return Cmp(lhs.s,lhs.n,rhs.s,rhs.n);
            }

            static inline 
            SignType Compare(const Integer &lhs, const Natural &rhs) noexcept
            {
                return Cmp(lhs.s,lhs.n,rhs<=0?__Zero__:Positive,rhs);
            }

            static inline 
            SignType Compare(const Natural &lhs, const Integer &rhs) noexcept
            {
                return Cmp(lhs<=0?__Zero__:Positive,lhs,rhs.s,rhs.n);
            }

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

#if 0
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
                Integer _ = Add(*this,rhs); xch(_);
                return *this;
            }

            //! in place +
            inline Integer & operator+=( const Natural &rhs ) {
                Integer _ = Add(s,n,Positive,rhs); xch(_);
                return *this;
            }

            //! in place +
            inline Integer & operator+=( const int64_t rhs ) {
                const Integer r(rhs);
                Integer _ = Add(*this,r);
                return *this;
            }

            //! +
            inline friend Integer operator+(const Integer &lhs, const Integer &rhs) {
                return Add(lhs,rhs);
            }

            //! +
            inline friend Integer operator+(const Integer &lhs, const Natural &rhs) {
                return Add(lhs.s,lhs.n,Positive,rhs);
            }

            //! +
            inline friend Integer operator+(const Natural &lhs, const Integer &rhs) {
                return Add(Positive,lhs,rhs.s,rhs.n);
            }


            //! +
            inline friend Integer operator+(const Integer &lhs, const int64_t rhs) {
                const Integer _(rhs);
                return Add(lhs,_);
            }

            //! +
            inline friend Integer operator+(const int64_t lhs, const Integer &rhs) {
                const Integer _(lhs);
                return Add(_,rhs);
            }

            //__________________________________________________________________
            //
            //
            // Subtractions
            //
            //__________________________________________________________________

            //! unary minus
            inline Integer operator-() const { return Integer( Sign::Opposite(s), n ); }
#endif
            
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
             \param rn rhs natural (Natural|uint64)
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




            static
            Integer Add(const SignType  ls,
                        const Natural  &ln,
                        const SignType  rs,
                        const Natural  &rn);

            static inline
            Integer Add(const Integer &lhs, const Integer &rhs) {
                return Add(lhs.s,lhs.n,rhs.s,rhs.n);
            }


            static inline
            Integer Sub(const SignType  ls,
                        const Natural  &ln,
                        const SignType  rs,
                        const Natural  &rn) { return Add(ls,ln, Sign::Opposite(rs), rn); }

            static inline
            Integer Sub(const Integer &lhs, const Integer &rhs) {
                return Sub(lhs.s,lhs.n,rhs.s,rhs.n);
            }
        };

    }

    typedef Kemp::Integer apz;

}

#endif

