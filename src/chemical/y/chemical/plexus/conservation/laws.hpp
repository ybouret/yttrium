
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            class Laws : public Proxy< const ListOf<Law> >, public Assembly
            {
            public:
                explicit Laws(Rule *);
                virtual ~Laws() noexcept;
                Y_OSTREAM_PROTO(Laws);

                void xch( Laws &other ) noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
                Y_PROXY_DECL();
                CxxListOf<Law> my;
            };

        }
    }
}


#endif
