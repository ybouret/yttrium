
#include "y/stream/hash/srzh.hpp"
#include "y/stream/hash/output.hpp"

namespace Yttrium
{
    SerializableHasher:: SerializableHasher(Hashing::Function &_) noexcept :
    hfcn(_)
    {

    }

    SerializableHasher:: SerializableHasher(const SerializableHasher &_) noexcept :
    hfcn(_.hfcn)
    {
    }

    SerializableHasher:: ~SerializableHasher() noexcept
    {

    }


    Hashing::Function & SerializableHasher:: hash(const Serializable &obj) noexcept
    {
        hfcn.set();
        HashingStream fp(hfcn);
        obj.serialize(fp);
        return hfcn;
    }


}
