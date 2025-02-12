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
            class Family : public Quantized, public Metrics, public Proxy<const Vector::List>
            {
            public:
                explicit Family(const Metrics &metrics,
                                const Cache   &sharedCache) noexcept;
                Family(const Family &);
                virtual ~Family() noexcept;
                Y_OSTREAM_PROTO(Family);


                template <typename T> inline
                bool wouldAccept(const Readable<T> &a)
                {
                    assert(a.size()==dimensions);

                    std::cerr << "wouldAccept " << a << "?" << std::endl;
                    // check dimension
                    if(qlist.size>=dimensions)
                        return false;

                    // check workspace
                    if(!qwork) qwork = cache->query();

                    // initialize
                    if(!qwork->set(a))
                        return false; // 0 vector

                    // remove projection
                    for(const Vector *basis=qlist.head;basis;basis=basis->next)
                    {
                        if(!qwork->keepOrtho(*basis)) return false;
                    }

                    std::cerr << "kept " << *qwork << std::endl;
                    return true;
                }

                void expand()
                {
                    assert(0!=qwork);
                    assert(qwork->ncof>0);
                    assert(qwork->nrm2>0);
                    assert(qlist.size<dimensions);

                    qlist.pushTail(qwork);
                    qwork = 0;
                    Coerce(quality) = getQuality(qlist.size);

                }


                void free() noexcept; //!< reset
                void trim() noexcept; //!< clear workspace

                const Quality quality;
            private:
                Y_DISABLE_ASSIGN(Family);
                Y_PROXY_DECL();
                Vector::List  qlist;
                Vector *      qwork;
                Cache         cache;



            public:
                Family *next;
                Family *prev;
            };
        }
    }
}

#endif

