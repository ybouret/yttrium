//! \file

#ifndef Y_Apex_Integer_Included
#define Y_Apex_Integer_Included 1

#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Apex
    {

        class Integer : public Number
        {
        public:
            Integer();
            virtual ~Integer() noexcept;
            Integer(const Integer &);
            Integer & operator=(const Integer &);
            Integer(const int64_t);
            Integer & operator=(const int64_t);
            Y_OSTREAM_PROTO(Integer);

            void xch(Integer &) noexcept;

            void appendTo(IO::Cache &) const;

            const SignType s;
            const Natural  n;
            
        };
    }

    typedef  Apex::Integer apz;
}

#endif
