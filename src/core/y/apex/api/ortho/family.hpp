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
                bool accept(const Readable<T> &a)
                {
                    QVector *qvec = cache->query();
                    try {
                        
                    }
                    catch(...) { cache->store(qvec); throw; }
                    return false;
                }


                template <typename T> inline
                bool initialize(const Readable<T> &first)
                {
                    QVector *qvec = cache->query();
                    try { qvec->set(first); }
                    catch(...) { cache->store(qvec); throw; }
                    if(qvec->ncof>0)
                    {
                        (void)qlist.pushTail(qvec);
                        return true;
                    }
                    else
                    {
                        cache->store(qvec);
                        return false;
                    }
                }


            private:
                Y_DISABLE_ASSIGN(QFamily);
                Y_PROXY_DECL();
                QVector::List                     qlist;
                CxxArray<Rational,Memory::Dyadic> qproj;
                QCache                            cache;


            public:
                QFamily *next;
                QFamily *prev;
            };
        }
    }
}

#endif

