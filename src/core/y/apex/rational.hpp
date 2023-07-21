
//! \file

#ifndef Y_Apex_Rational_Included
#define Y_Apex_Rational_Included 1

#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {

        class Rational : public Number
        {
        public:
            static const char * const CallSign;

            Rational();
            Rational(const int64_t, const uint64_t);
            virtual ~Rational() noexcept;
            Y_OSTREAM_PROTO(Rational);


            void appendTo(IO::Chars &) const;

            const Integer numer;
            const Natural denom;

        private:
            void zDenom() const;
            void update();
        };


    }

    typedef Apex::Rational apq;
}
#endif

