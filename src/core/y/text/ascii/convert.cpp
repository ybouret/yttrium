
#include "y/text/ascii/convert.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace ASCII
    {


        void Convert::Check(const uint64_t value, const uint64_t vmax, const char *ctx)
        {
            if(value>vmax)
                throw Specific::Exception("Convert To Unsigned","result too large for %s",(ctx?ctx:Core::Unknown));
        }

        void Convert::Check(const int64_t vmin, const int64_t value, const int64_t vmax, const char *ctx)
        {
            if(value<vmin||value>vmax)
                throw Specific::Exception("Convert To Signed","result too large for %s",(ctx?ctx:Core::Unknown));
        }


    }

}

