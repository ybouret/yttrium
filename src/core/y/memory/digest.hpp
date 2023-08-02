//! \file

#ifndef Y_Memory_Digest_Included
#define Y_Memory_Digest_Included 1

#include "y/container/writable.hpp"
#include "y/memory/buffer/rw.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    class Digest :
    public Memory::Wad<uint8_t,Memory::Dyadic>,
    public Writable<uint8_t>,
    public Memory::ReadWriteBuffer
    {
    public:
        typedef Memory::Wad<uint8_t,Memory::Dyadic> WadType;

        explicit Digest(const size_t);
        virtual ~Digest() noexcept;


    };
}
#endif

