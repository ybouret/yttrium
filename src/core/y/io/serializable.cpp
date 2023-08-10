
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

#include "y/string.hpp"
#include "y/io/data/output.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    String Serializable:: toBinary() const
    {
        OutputDataStream<String> fp;
        const size_t nw = serialize(fp);
        if(fp.size()!=nw) throw Specific::Exception("Serialize.toBinary","sizes mismatch");
        return fp;
    }
}
