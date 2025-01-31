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

            Rational(const integer_t );
            Rational(const Rational &);
            Rational(const Natural  &);
            Rational(const Integer  &);
            Rational(const integer_t, const natural_t);
            Rational(const Integer &, const Natural &);
            virtual ~Rational() noexcept;

            Rational & operator=(const Rational &);
            Rational & operator=(const integer_t );

            Y_OSTREAM_PROTO(Rational);

            Rational & xch(Rational &q);
            void       simplify();

            //
            virtual const char * callSign() const noexcept;
            virtual size_t       serialize(OutputStream &) const;
            static  Rational     Read(InputStream &);


            // comparison
            friend bool operator==(const Rational &, const Rational &) noexcept;
            friend bool operator!=(const Rational &, const Rational &) noexcept;
            


            // Members
            const Integer numer;
            const Natural denom;

        private:
            
        };
    }

}


#endif
