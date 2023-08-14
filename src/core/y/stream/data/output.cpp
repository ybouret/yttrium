
#include "y/stream/data/output.hpp"

namespace Yttrium
{
    namespace Core
    {
        OutputDataStream:: OutputDataStream() noexcept : OutputStream()
        {
        }

        OutputDataStream:: ~OutputDataStream() noexcept
        {
        }

        const char * const OutputDataStream:: CallSign = "OutputDataStream";

        const char * OutputDataStream:: callSign() const noexcept
        {
            return CallSign;
        }

        void OutputDataStream:: flush() {}
    }

}
