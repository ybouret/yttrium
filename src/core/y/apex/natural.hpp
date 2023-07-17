//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/number.hpp"
#include "y/ostream-proto.hpp"
#include "y/config/shallow.hpp"

namespace Yttrium
{
    namespace Random { class Bits; }


    namespace Apex
    {

        Y_SHALLOW_DECL(AsImpl);

        //______________________________________________________________________
        //
        //
        //
        //! Drop-in Unsigned Type replacement
        //
        //
        //______________________________________________________________________
        class Natural : public Number
        {
        public:
            Natural();
            Natural(const uint64_t);
            Natural(const Natural &);
            Natural(const size_t n, Random::Bits &);
            Natural &operator=(const Natural &);
            Natural &operator=(const uint64_t) noexcept;
            virtual ~Natural() noexcept;
            Y_OSTREAM_PROTO(Natural);

            size_t         bytes() const noexcept;
            size_t         bits()  const noexcept;
            void           xch(Natural &) noexcept;
            std::ostream & printHex(std::ostream &) const;

            operator uint64_t() const noexcept;


            // ADD
            friend Natural operator+(const Natural &lhs, const Natural &rhs);
            friend Natural operator+(const Natural &lhs, const uint64_t rhs);
            friend Natural operator+(const uint64_t lhs, const Natural &rhs);
            Natural & operator+=(const Natural&);
            Natural & operator+=(const uint64_t);
            Natural & operator++();
            Natural   operator++(int);

            // SUB
            friend Natural operator-(const Natural &lhs, const Natural &rhs);
            friend Natural operator-(const Natural &lhs, const uint64_t rhs);
            friend Natural operator-(const uint64_t lhs, const Natural &rhs);
            Natural & operator-=(const Natural&);
            Natural & operator-=(const uint64_t);
            Natural & operator--();
            Natural   operator--(int);

        private:
            void *impl;
            Natural(void*,const AsImpl_&) noexcept;
            
        };

    }

    typedef Apex::Natural APN; //!< alias

}

#endif

