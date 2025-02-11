
#include "y/apex/api/ortho/vector.hpp"
#include "y/apex/api/univocal.hpp"
#include "y/apex/api/count-non-zero.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            QVector:: ~QVector() noexcept {}


            QVector:: QVector(const size_t dims) :
            Quantized(),
            Metrics(dims),
            QVectorType(dims),
            ncof(0),
            nrm2(0),
            next(0),
            prev(0)
            {
            }


            QVector & QVector:: ldz() noexcept
            {
                Coerce(ncof) = 0;
                Coerce(nrm2) = 0;
                for(size_t i=dimensions;i>0;--i)
                {
                    Coerce((*this)[i])  = 0;
                }
                return *this;
            }

            void QVector:: set(Writable<Rational> &Q)
            {
                assert(Q.size() == dimensions);
                try {
                    Coerce(nrm2) = Univocal::Make(Q);
                    Coerce(ncof) = 0;
                    for(size_t i=dimensions;i>0;--i)
                    {
                        assert(Q[i].denom==1);
                        switch( (Coerce( (*this)[i] ) = Q[i].numer).s )
                        {
                            case __Zero__: continue;
                            case Positive:
                            case Negative:
                                ++Coerce(ncof);
                                continue;;
                        }
                    }
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

            QVector:: Cache:: ~Cache() noexcept
            {

            }

            QVector:: Cache:: Cache(const size_t dims) noexcept :
            Metrics(dims),
            Proxy<const Pool>(),
            my()
            {
            }

            Y_PROXY_IMPL(QVector:: Cache,my)

            QVector * QVector:: Cache:: query()
            {
                return my.size <= 0 ? new QVector(dimensions) : my.query();
            }

            void QVector:: Cache:: store(QVector * const v) noexcept
            {
                assert(0!=v);
                my.store(v)->ldz();
            }

        }
    }
}

