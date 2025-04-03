
#include "y/apex/api/ortho/architect.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {

            Architect:: ~Architect() noexcept
            {
                
            }

            Architect:: Architect(const size_t dims) :
            Metrics(dims),
            vcache( new Vector::Cache(*this) ),
            family( vcache ),
            ivList( dims )
            {
                vcache->reserve(dims);
            }


            void Architect:: record(const size_t i)
            {
                assert( ! ivList.has(i) );
                const Vector * const vec = family.increase();
                if(0==vec) throw Specific::Exception("Ortho::Architect", "corrupted inserted row[%u]", unsigned(i));
                ivList << i;
            }


            void Architect:: clear() noexcept { family.reset(); ivList.free(); }

        }

    }

}
