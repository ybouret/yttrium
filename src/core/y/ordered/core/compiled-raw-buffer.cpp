
#include "y/ordered/core/compiled-raw-buffer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace Core
    {

        void CompiledRawBuffer_:: RaiseExceeded(const size_t n)
        {
            throw Specific::Exception("CompileRawBuffer","exceeded capacity=%u", unsigned(n));
        }


    }

}

