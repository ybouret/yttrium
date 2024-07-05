
#include "y/chemical/indexed.hpp"
#include "y/system/exception.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {

        Indexed:: ~Indexed() noexcept
        {
        }


        void Indexed:: setAllIndices(const size_t level)
        {
            if(level<=0) throw Specific::Exception("Chemical::Entity", "'%s' with zero index", name.c_str());
            for(unsigned i=0;i<Levels;++i) Coerce(indx[i]) = level;
        }

    }

}
