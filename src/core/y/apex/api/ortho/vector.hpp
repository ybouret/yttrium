//! \file

#ifndef Y_Apex_Ortho_Vector_Included
#define Y_Apex_Ortho_Vector_Included 1

#include "y/apex/api/ortho/metrics.hpp"
#include "y/apex/rational.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/container/cxx/array.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {

            typedef CxxArray<const Integer,Memory::Dyadic> QVectorType;

            class QVector : public Quantized, public Metrics, public QVectorType
            {
            public:

                // Definitions
                typedef CxxPoolOf<QVector> Pool;
                typedef CxxListOf<QVector> List;

                class Cache : public Object, public Counted, public Metrics, public Proxy<const Pool>
                {
                public:
                    explicit Cache(const Metrics &m) noexcept;
                    virtual ~Cache()                 noexcept;

                    QVector *query();
                    void     store(QVector * const) noexcept;
                    QVector *query(const QVector &);


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Cache);
                    Y_PROXY_DECL();
                    Pool my;
                };

                // C++
                explicit QVector(const Metrics &);
                virtual ~QVector() noexcept;


                // Methods
                QVector & ldz() noexcept;
                void      set(Writable<Rational> &Q);
                

                // Members
                const size_t  ncof; //!< number of non-zero coefficients
                const Natural nrm2; //!< |this|^2
                QVector *     next;
                QVector *     prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(QVector);
            };

            typedef ArcPtr<QVector::Cache> QCache;
        }

    }

}

#endif

