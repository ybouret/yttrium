
#include "y/chemical/species.hpp"
#include "y/stream/output.hpp"
#include "y/stream/input.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Species:: ~Species() noexcept
        {

        }

        size_t Species:: serialize(OutputStream &fp) const
        {
            size_t ans = name.serialize(fp);
            ans += fp.emitVBR(z);
            return ans;
        }

        Species * Species:: Read(InputStream &fp, const size_t _indx)
        {
            const String _name = String::ReadFrom(fp,"Species.name");
            const int    _z    = fp.readVBR<int>("Species.z");
            return new Species(_name,_z,_indx);
        }

    }

}
