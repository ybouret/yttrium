
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/reactive/conservation/law.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Grouping;

        namespace Conservation
        {

            class Laws : public Object, public Law::List
            {
            public:
                static const char * const CallSign;
                virtual ~Laws() noexcept;
                explicit Laws(const Grouping &, XMLog &xml);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);

            };


        }

    }


}

#endif

