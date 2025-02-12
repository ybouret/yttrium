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
            class QFamily : public Quantized, public Metrics, public Proxy<const QVector::List>
            {
            public:
                explicit QFamily(const Metrics &metrics,
                                 const QCache  &sharedCache) noexcept;
                QFamily(const QFamily &);
                virtual ~QFamily() noexcept;

                template <typename T> inline
                bool wouldAccept(const Readable<T> &a)
                {
                    assert(a.size()==dimensions);

                    // check dimension
                    if(qlist.size>=dimensions)
                        return false;

                    // check workspace
                    if(!qwork) qwork = cache->query();
                    QVector   &v     = *qwork;

                    // initialize
                    if(!v.set(a)) return false; // 0 vector

                    return false;
                }

                void free() noexcept;
                void trim() noexcept;

            private:
                Y_DISABLE_ASSIGN(QFamily);
                Y_PROXY_DECL();
                QVector::List                     qlist;
                QVector *                         qwork;
                QCache                            cache;


            public:
                QFamily *next;
                QFamily *prev;
            };
        }
    }
}

#endif

