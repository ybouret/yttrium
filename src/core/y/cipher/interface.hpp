//! \file

#ifndef Y_Cipher_Included
#define Y_Cipher_Included 1

#include "y/type/identifiable.hpp"
#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{
    
    //! base class for ciphers
    class Cipher : public Identifiable
    {
    public:
        typedef Memory::ReadOnlyBuffer Key;

    protected:
        explicit Cipher() noexcept;

    public:
        virtual ~Cipher() noexcept;

        virtual void schedule(const Key &key) noexcept = 0;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Cipher);
    };

}

#endif

