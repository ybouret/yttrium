
#include "y/chemical/type/indexed.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Indexed:: Indexed(const size_t topLevelIndex) noexcept :
        Entity(),
        indx()
        {
            assert(topLevelIndex>0);
            for(size_t i=0;i<Levels;++i) Coerce(indx[i]) = topLevelIndex;
        }

        Indexed:: ~Indexed() noexcept
        {

        }





    }

}

#include "y/graphviz/vizible.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        String Indexed:: makeColorFrom(const char * const scheme) const
        {
            const String cid = GraphViz::Vizible::Color(scheme,indx[TopLevel]);
            return  cid;
        }
    }
}
