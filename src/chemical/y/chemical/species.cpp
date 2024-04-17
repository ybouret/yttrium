
#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        
        Species::~Species() noexcept
        {
        }

        void Species:: viz(OutputStream &fp) const
        {
            Node(fp,this) << '[';
            Label(fp,name) << ",shape=egg";
            Endl(fp<<']');
        }

    }
}
