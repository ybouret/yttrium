
#include "y/stream/serializer.hpp"

namespace Yttrium
{
    namespace IO
    {
        Bytes:: ~Bytes() noexcept { ld(0); }
        Bytes::  Bytes() noexcept : BytesField()  {}

        const void * Bytes:: ro_addr() const noexcept { const BytesField &self = *this; return self(); }
        size_t       Bytes:: measure() const noexcept { return size(); }
    }

    Serializer::  Serializer() noexcept:
    iodata(),
    stream(iodata)
    {
    }

    Serializer:: ~Serializer() noexcept {}
    
}
