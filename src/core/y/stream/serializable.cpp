
#include "y/stream/serializable.hpp"
#include "y/stream/output.hpp"
#include "y/text/ops.hpp"

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
        fp.frame(msg,len);
        return len;
    }
}

#include "y/string.hpp"
#include "y/stream/data/output.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    String Serializable:: toBinary() const
    {
        String bin;
        {
            OutputDataStream<String> fp(bin);
            const size_t nw = serialize(fp);
            if(bin.size()!=nw) throw Specific::Exception("Serialize.toBinary","sizes mismatch");
        }
        return bin;
    }
}
#include "y/hashing/function.hpp"
#include "y/stream/hash/output.hpp"

namespace Yttrium
{

    void Serializable:: runHash( Hashing::Function &usr ) const noexcept
    {
        HashingStream fp(usr);
        (void)serialize(fp);
    }
}

#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    size_t Serializable:: toBinary(const String &fileName, const bool append) const
    {
        OutputFile fp(fileName,append);
        return serialize(fp);
    }

    size_t Serializable:: toBinary(const char *fileName, const bool append) const
    {
        const String _(fileName);
        return toBinary(_,append);
    }

}
