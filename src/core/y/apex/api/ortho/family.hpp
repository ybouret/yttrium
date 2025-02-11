//! \file

#ifndef Y_Apex_Ortho_Family_Included
#define Y_Apex_Ortho_Family_Included 1

#include "y/apex/api/ortho/vector.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {
            class QFamily : public Metrics, public Proxy<const QVector::List>
            {
            public:
                explicit QFamily(const Metrics &metrics,
                                 const QCache  &sharedCache) noexcept;
                QFamily(const QFamily &);
                virtual ~QFamily() noexcept;

            private:
                Y_DISABLE_ASSIGN(QFamily);
                Y_PROXY_DECL();
                QVector::List qlist;
                QCache        cache;


            public:
                QFamily *next;
                QFamily *prev;
            };
        }
    }
}

#endif

