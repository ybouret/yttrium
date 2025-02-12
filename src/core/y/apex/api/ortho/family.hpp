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
            class QFamily : public Quantized, public Metrics, public Proxy<const Vector::List>
            {
            public:
                explicit QFamily(const Metrics &metrics,
                                 const Cache   &sharedCache) noexcept;
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
                    Vector    &v     = *qwork;

                    // initialize
                    if(!v.set(a))
                        return false; // 0 vector

                    // remove projection
                    for(const Vector *basis=qlist.head;basis;basis=basis->next)
                    {
                        if(!v.keepOrtho(*basis)) return false;
                    }


                    return true;
                }

                void free() noexcept; //!< reset
                void trim() noexcept; //!< clear workspace

            private:
                Y_DISABLE_ASSIGN(QFamily);
                Y_PROXY_DECL();
                Vector::List  qlist;
                Vector *      qwork;
                Cache         cache;


            public:
                QFamily *next;
                QFamily *prev;
            };
        }
    }
}

#endif

