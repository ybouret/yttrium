
#include "y/io/serializable.hpp"
#include "y/text/ops.hpp"
#include "y/io/stream/output.hpp"

namespace Yttrium
{
    Serializable:: ~Serializable() noexcept
    {
    }

    Serializable:: Serializable() noexcept
    {
    }

    size_t Serializable::emitMessage(OutputStream &fp, const char *msg) const
    {
        const size_t len = StringLength(msg);
        fp.write(msg,len);
        return len;
    }
}

