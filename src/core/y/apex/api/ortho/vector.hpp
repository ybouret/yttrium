//! \file

#ifndef Y_Apex_Ortho_Vector_Included
#define Y_Apex_Ortho_Vector_Included 1

#include "y/apex/api/ortho/metrics.hpp"
#include "y/apex/api/univocal.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/container/cxx/array.hpp"
#include "y/data/list.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {

            //__________________________________________________________________
            //
            //! base type for Vector
            //__________________________________________________________________
            typedef CxxArray<const Integer,Memory::Dyadic> VectorType;

            //__________________________________________________________________
            //
            //
            //
            //! Fixed Length Vector
            //
            //
            //__________________________________________________________________
            class Vector : public Object, public Metrics, public VectorType
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxPoolOf<Vector> Pool;    //!< alias
                typedef ListOf<Vector>    List;    //!< alias
                typedef Writable<Integer> Array;   //!< alias

                //______________________________________________________________
                //
                //
                //! cache of vectors with same metrics
                //
                //______________________________________________________________
                class Cache : public Object, public Counted, public Metrics, public Proxy<const Pool>
                {
                public:
                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    explicit Cache(const Metrics &m) noexcept; //!< setup
                    virtual ~Cache()                 noexcept; //!< cleanup

                    //__________________________________________________________
                    //
                    // Methods
                    //__________________________________________________________
                    Vector *query();                         //!< query/create
                    void    store(Vector * const) noexcept;  //!< store and reset
                    Vector *query(const Vector &);           //!< duplicate
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Cache);
                    Y_PROXY_DECL();
                    Pool my;
                };

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Vector(const Metrics &); //!< setup
                virtual ~Vector() noexcept;       //!< cleanup
                Y_OSTREAM_PROTO(Vector);          //!< display

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void  ldz() noexcept;      //!< reset all
                void  ld(const Vector &); //!< duplicate


                //! transfer compatible vector, return true if not nul vector
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

                //! dot product
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

                //! keep orthogonal to e, return true if not nul vector
                bool keepOrtho(const Vector &e);

                static SignType Compare(const Vector &lhs, const Vector &rhs) noexcept;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const size_t  ncof; //!< number of non-zero coefficients
                const Integer nrm2; //!< |this|^2, as integer for algebraic operations
                Vector *      next; //!< for list/pool
                Vector *      prev; //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vector);
                bool   finalize(Array &self); //!< finalize from ncof
                Array &get() noexcept;        //!< get aliases
            };

            //__________________________________________________________________
            //
            //! shared cache
            //__________________________________________________________________
            typedef ArcPtr<Vector::Cache> VCache;
        }

    }

}

#endif

