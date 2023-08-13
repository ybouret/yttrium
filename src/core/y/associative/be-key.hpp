//! \file

#ifndef Y_Associative_BE_Key_Included
#define Y_Associative_BE_Key_Included 1

#include "y/memory/buffer/ro.hpp"
#include "y/type/ints.hpp"

namespace Yttrium
{

    class OutputStream;

    class BE_Key : public Memory::ReadOnlyBuffer
    {
    public:
        template <typename T>
        inline BE_Key(T &args) noexcept :
        size(sizeof(T)),
        code()
        {
            union {
                T                                  data;
                typename IntegerFor<T>::UInt::Type uint;
            } alias = { args };
            setup(alias.uint);
        }

        BE_Key(const BE_Key &) noexcept;
        virtual ~BE_Key()      noexcept;

        virtual const void * ro_addr() const noexcept;
        virtual size_t       measure() const noexcept;

        friend std::ostream & operator<<(std::ostream &os, const BE_Key &key);
        friend OutputStream & operator<<(OutputStream &os, const BE_Key &key);

    private:
        Y_DISABLE_ASSIGN(BE_Key);
        const size_t size;
        uint8_t      code[8];
        void setup(const uint8_t) noexcept;
        void setup(const uint16_t) noexcept;
        void setup(const uint32_t) noexcept;
        void setup(const uint64_t) noexcept;

        
    };

}

#endif

