//! \file

#ifndef Y_Apex_Ortho_Vector_Included
#define Y_Apex_Ortho_Vector_Included 1

#include "y/apex/api/ortho/metrics.hpp"
#include "y/apex/api/univocal.hpp"
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
                typedef Writable<Integer>  Array;

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
                //void      set(Writable<Rational> &Q);

                
                template <typename T> inline
                bool set(const Readable<T> &V)
                {
                    assert(V.size()==dimensions);
                    try {
                        Array &self  = get();
                        Coerce(ncof) = 0;
                        for(size_t i=dimensions;i>0;--i)
                        {
                            switch( (self[i] = V[i]).s )
                            {
                                case __Zero__: continue;
                                case Positive:
                                case Negative: ++Coerce(ncof); continue;
                            }
                        }
                        return finalize(self);
                    }
                    catch(...)
                    {
                        ldz();
                        throw;
                    }
                }

                template <typename T> inline
                Integer dot(const Readable<T> &a) const
                {
                    assert(a.size()==dimensions);
                    Integer res;
                    const Readable<const Integer> &self = *this;
                    for(size_t i=dimensions;i>0;--i)
                    {
                        res += self[i] * a[i];
                    }
                    return res;
                }

                //! |e|^2 * a - (a.e) * e
                template <typename T> inline
                bool set(const QVector &e, const Readable<T> &a)
                {
                    assert(a.size()==dimensions);
                    assert(e.ncof>0);
                    assert(e.nrm2>0);
                    try {
                        Array &         self = get();
                        const Integer & wa   = e.nrm2;
                        const Integer   we   = e.dot(a);
                        Coerce(ncof) = 0;
                        for(size_t i=dimensions;i>0;--i)
                        {
                            switch( (self[i] = (wa * a[i]) - we * e[i]).s )
                            {
                                case __Zero__: continue;
                                case Positive:
                                case Negative: ++Coerce(ncof); continue;
                            }
                        }
                        return finalize(self);
                    }
                    catch(...)
                    {
                        ldz();
                        throw;
                    }
                }



                // Members
                const size_t  ncof; //!< number of non-zero coefficients
                const Integer nrm2; //!< |this|^2, as integer for algebraic operations
                QVector *     next;
                QVector *     prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(QVector);
                bool   finalize(Array &self); //!< finalize from ncof
                Array &get() noexcept;        //!< get aliases
            };

            typedef ArcPtr<QVector::Cache> QCache;
        }

    }

}

#endif

