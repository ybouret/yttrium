
//! \file

#ifndef Y_Apex_Ortho_Families_Included
#define Y_Apex_Ortho_Families_Included 1

#include "y/apex/api/ortho/family.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {

            class Families  : public Metrics, public Proxy<const Family::List>
            {
            public:
                explicit Families(const Metrics &, const FCache &) noexcept;
                virtual ~Families() noexcept;

                void clear() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Families);
                Y_PROXY_DECL();
                Family::List my;
                FCache       fc;
            };

        }

    }

}

#endif

