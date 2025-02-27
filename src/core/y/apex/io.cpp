
#include "y/apex/io.hpp"

namespace Yttrium
{

    namespace Apex
    {
        void IOBits:: Store(StreamBits &io, const Natural &n)
        {
            Y_Apex_Lock(n);
            Block &      blk = Coerce(*n).to(Plan1);
            const size_t num = blk.bits;
            for(size_t i=0;i<num;++i)
            {
                if( blk.get_(i) ) io << IO::_1; else io << IO::_0;
            }
        }
    }

}
