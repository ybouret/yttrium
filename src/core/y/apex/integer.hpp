//! \file

#ifndef Y_Apex_Integer_Included
#define Y_Apex_Integer_Included 1

#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Apex
    {
        //______________________________________________________________________
        //
        //
        //
        //! Drop-in Signed Type replacement
        //
        //
        //______________________________________________________________________
        class Integer : public Number
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Apex::Natural"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Integer();                             //!< zero
            virtual ~Integer() noexcept;           //!< cleanup

            Integer(const Integer &);              //!< copy   integer
            Integer & operator=(const Integer &);  //!< assign integer

            Integer(const Natural &);              //!< setup natural
            Integer & operator=(const Natural &);  //!< assign natural

            Integer(const int64_t);                //!< setup integral
            Integer & operator=(const int64_t);    //!< assign integral

            Y_OSTREAM_PROTO(Integer);              //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void xch(Integer &)     noexcept;  //!< no-throw exchange
            void appendTo(IO::Chars &) const;  //!< append to chars


            //__________________________________________________________________
            //
            //
            // Addition
            //
            //__________________________________________________________________
            Integer operator+() const; //!< unary plus
            friend Integer operator+(const Integer &lhs, const Integer &rhs);

            //__________________________________________________________________
            //
            //
            // Subtraction
            //
            //__________________________________________________________________
            Integer operator-() const; //! unary minus

            //__________________________________________________________________
            //
            //
            // Multiplication
            //
            //__________________________________________________________________
            friend Integer operator*(const Integer &, const Integer &);
            friend Integer operator*(const Integer &, const int64_t  );
            friend Integer operator*(const int64_t  , const Integer &);
            friend Integer operator*(const Integer &, const Natural &);
            friend Integer operator*(const Natural &, const Integer &);
            Integer & operator*=(const Integer &);
            Integer & operator*=(const int64_t  );
            Integer & operator*=(const Natural &);

            //__________________________________________________________________
            //
            //
            // Division
            //
            //__________________________________________________________________
            friend Integer operator/(const Integer &, const Integer &);
            friend Integer operator/(const Integer &, const int64_t  );
            friend Integer operator/(const int64_t  , const Integer &);
            friend Integer operator/(const Integer &, const Natural &);
            friend Integer operator/(const Natural &, const Integer &);
            Integer & operator/=(const Integer &);
            Integer & operator/=(const int64_t  );
            Integer & operator/=(const Natural &);

            //__________________________________________________________________
            //
            //
            // comparison
            //
            //__________________________________________________________________
            static SignType Compare(const Integer &lhs, const Integer &rhs) noexcept;
            friend bool operator==(const Integer &lhs, const Integer &rhs) noexcept;
            friend bool operator!=(const Integer &lhs, const Integer &rhs) noexcept;

            friend bool operator==(const Integer &lhs, const int64_t  rhs) noexcept;



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SignType s; //!< sign
            const Natural  n; //!< unsigned value

        private:
            Integer(const SignType _s, const Natural _n);
            static SignType Cmp(const Integer &lhs, const Integer &rhs) noexcept;
            static SignType Cmp(const Integer &lhs, const int64_t  rhs) noexcept;

        };
    }

    typedef  Apex::Integer apz; //!< alias
}

#endif
