
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
            static SignType Compare(const Integer &lhs, const Integer &rhs);

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
            static
            SignType Cmp(const SignType  ls,
                         const Natural  &ln,
                         const SignType  rs,
                         const Natural  &rn) noexcept;

            static inline
            SignType Cmp(const Integer &lhs, const Integer &rhs) noexcept
            {
                return Cmp(lhs.s,lhs.n,rhs.s,rhs.n);
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

