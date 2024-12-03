
#include "y/chemical/type/indexed.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Indexed:: Indexed(const size_t topIndex) noexcept :
        Entity(),
        indx()
        {
            assert(topIndex>0);
            for(size_t i=0;i<Levels;++i) Coerce(indx[i]) = 0;
            Coerce(indx[TopLevel]) = topIndex;
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
