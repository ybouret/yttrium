//! \file

#ifndef Y_Memory_Digest_Included
#define Y_Memory_Digest_Included 1

#include "y/container/writable.hpp"
#include "y/memory/buffer/rw.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    class Digest :
    public Writable<uint8_t>,
    public Memory::ReadWriteBuffer
    {
    public:
        static const char * const CallSign;

        explicit Digest(const size_t);
        virtual ~Digest() noexcept;
        Digest(const Digest &);
        Digest(const char *hexa);
        Y_OSTREAM_PROTO(Digest);

        virtual const char *callSign() const throw();
        virtual size_t      size()     const throw();
        virtual const void *ro_addr()  const throw();
        virtual size_t      measure()  const throw();

        uint8_t &       operator[](const size_t)       noexcept;
        const uint8_t & operator[](const size_t) const noexcept;

    private:
        Y_DISABLE_ASSIGN(Digest);
        class Code;
        Code *code;
        

    };
}
#endif

