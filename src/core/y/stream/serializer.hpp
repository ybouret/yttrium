
//! \file

#ifndef Y_IO_Serializer_Included
#define Y_IO_Serializer_Included 1

#include "y/stream/serializable.hpp"
#include "y/stream/data/output.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{

    namespace IO
    {
        typedef Vector<uint8_t,Memory::Dyadic> BytesField;

        class Bytes : public Memory::ReadOnlyBuffer, public BytesField
        {
        public:
            explicit Bytes() noexcept;
            virtual ~Bytes() noexcept;

            virtual const void *ro_addr() const noexcept;
            virtual size_t      measure() const noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bytes);
        };

        typedef OutputDataStream<Bytes> OutputBytes;
    }

    class Serializer
    {
    public:
        explicit Serializer() noexcept;
        virtual ~Serializer() noexcept;

        IO::Bytes       iodata;
        IO::OutputBytes stream;

        size_t vbr(const Serializable &srz) { return srz.serialize(stream); }
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Serializer);
    };

}

#endif

