
#include "y/apex/api/ortho/vector.hpp"
#include "y/apex/api/univocal.hpp"
#include "y/apex/api/count-non-zero.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            Vector:: ~Vector() noexcept {}


            Vector:: Vector(const Metrics &m) :
            Quantized(),
            Metrics(m),
            VectorType(dimensions),
            ncof(0),
            nrm2(0),
            next(0),
            prev(0)
            {
            }


            void Vector:: ldz() noexcept
            {
                Coerce(ncof) = 0;
                Coerce(nrm2) = 0;
                for(size_t i=dimensions;i>0;--i)
                {
                    Coerce((*this)[i])  = 0;
                }
            }

            Vector::Array & Vector:: get() noexcept
            {
                Writable<const Integer> &self = *this;
                return (Array &)self;
            }

            bool Vector:: finalize(Array &self)
            {
                if(ncof<=0)
                {
                    Coerce(nrm2) = 0;
                    return false;
                }
                else
                {
                    Coerce(nrm2) = Univocal::Make(self); assert(nrm2>0);
                    return true;
                }
            }

            std::ostream & operator<<(std::ostream &os, const Vector &v)
            {
                const VectorType &base = v;
                os << base << " #" << v.ncof << "@" << v.nrm2;
                return os;
            }

            bool Vector:: keepOrtho(const Vector &e)
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



        }

    }

}


namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {

            Vector:: Cache:: ~Cache() noexcept
            {

            }

            Vector:: Cache:: Cache(const Metrics &m) noexcept :
            Metrics(m),
            Proxy<const Pool>(),
            my()
            {
            }

            Y_PROXY_IMPL(Vector:: Cache,my)

            Vector * Vector:: Cache:: query()
            {
                return my.size <= 0 ? new Vector(*this) : my.query();
            }

            void Vector:: Cache:: store(Vector * const v) noexcept
            {
                assert(0!=v);
                my.store(v)->ldz();
            }

            Vector * Vector:: Cache:: query(const Vector &V)
            {
                Vector *qvec = query();
                try {

                    Coerce(qvec->ncof) = V.ncof;
                    Coerce(qvec->nrm2) = V.nrm2;
                    for(size_t i=dimensions;i>0;--i)
                        Coerce( (*qvec)[i] ) = V[i];
                    return qvec;
                }
                catch(...)
                {
                    store(qvec);
                    throw;
                }
            }


        }
    }
}

