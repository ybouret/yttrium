
//! \file

#ifndef Y_Apex_Jig_API_Included
#define Y_Apex_Jig_API_Included 1

#include "y/apex/types.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    namespace Apex
    {
        class JigAPI
        {
        public:
            static const unsigned Plans = 4;
            static const unsigned Faded = Plans-1;
            static const Plan     Dull[Plans][Faded];

        protected:
            explicit JigAPI(const size_t _count) noexcept;

        public:
            virtual ~JigAPI() noexcept;

            virtual void   setBits(const size_t bits)       noexcept = 0;
            virtual size_t upgrade()                        noexcept = 0;
            virtual void   display(std::ostream &)    const          = 0;
            virtual bool   chkBits(const size_t bits) const noexcept = 0;
            Y_OSTREAM_PROTO(JigAPI);

        public:
            const size_t words;
            const size_t count;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(JigAPI);
        };

    }

}

#endif

