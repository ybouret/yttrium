
#include "y/chemical/species.hpp"
#include "y/stream/output.hpp"
#include "y/stream/input.hpp"

#include "y/chemical/type/io.hpp"

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

        const char * const Species:: CallSign = "Chemical::Species";
        
        Species * Species:: Read(InputStream &fp, const size_t _indx)
        {
            VarInfo       info;
            const String _name = String::ReadFrom(fp,info("%s[%u].name",CallSign,unsigned(_indx)) );
            const int    _z    = fp.readVBR<int>(info("%s.z",_name.c_str()));
            return new Species(_name,_z,_indx);
        }

    }

}
