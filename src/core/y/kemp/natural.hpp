//! \file

#ifndef Y_Kemp_Natural_Included
#define Y_Kemp_Natural_Included 1

#include "y/kemp/number.hpp"
#include "y/kemp/types.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        class Element;

        class Natural : public Number
        {
        public:
            virtual ~Natural() noexcept;
            Y_OSTREAM_PROTO(Natural);
            
            Natural();
            
            Natural(const Natural &);
            Natural(const uint64_t);
            
            Natural(const size_t nbits, Random::Bits &);
            Natural(const TwoToThe_ &, const size_t ibit);

            Natural &operator=(const Natural &);
            Natural &operator=(const uint64_t);
            

            void xch(Natural &) noexcept;
            String toHex() const;


        private:
            mutable Element *code;
        };

    }
    typedef Kemp::Natural apn;
    
}

#endif

