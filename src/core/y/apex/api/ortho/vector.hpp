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

            typedef CxxArray<const Integer,Memory::Dyadic> VectorType;

            class Vector : public Quantized, public Metrics, public VectorType
            {
            public:

                // Definitions
                typedef CxxPoolOf<Vector> Pool;
                typedef ListOf<Vector>    List;
                typedef Writable<Integer> Array;

                class Cache : public Object, public Counted, public Metrics, public Proxy<const Pool>
                {
                public:
                    explicit Cache(const Metrics &m) noexcept;
                    virtual ~Cache()                 noexcept;

                    Vector *query();
                    void    store(Vector * const) noexcept;
                    Vector *query(const Vector &);


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Cache);
                    Y_PROXY_DECL();
                    Pool my;
                };

                // C++
                explicit Vector(const Metrics &);
                virtual ~Vector() noexcept;
                Y_OSTREAM_PROTO(Vector);

                // Methods
                void  ldz() noexcept;


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

                bool keepOrtho(const Vector &e)
                {
                    assert(e.dimensions==dimensions);
                    try {
                        Array &         a  = get();
                        const Integer & wa = e.nrm2;
                        const Integer   we = e.dot(a);
                        Coerce(ncof) = 0;
                        for(size_t i=dimensions;i>0;--i)
                        {
                            switch( (a[i] = (wa * a[i]) - we * e[i]).s )
                            {
                                case __Zero__: continue;
                                case Positive:
                                case Negative: ++Coerce(ncof); continue;
                            }
                        }
                        return finalize(a);
                    }
                    catch(...)
                    {
                        ldz();
                        throw;
                    }
                }


#if 0
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
#endif


                // Members
                const size_t  ncof; //!< number of non-zero coefficients
                const Integer nrm2; //!< |this|^2, as integer for algebraic operations
                Vector *     next;
                Vector *     prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vector);
                bool   finalize(Array &self); //!< finalize from ncof
                Array &get() noexcept;        //!< get aliases
            };

            typedef ArcPtr<Vector::Cache> Cache;
        }

    }

}

#endif

