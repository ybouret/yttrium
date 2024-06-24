//! \file

#ifndef Y_Cipher_Included
#define Y_Cipher_Included 1

#include "y/type/identifiable.hpp"

namespace Yttrium
{

    //! base class for ciphers
    class Cipher : public Identifiable
    {
    protected:
        explicit Cipher() noexcept;

    public:
        virtual ~Cipher() noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Cipher);
    };

}

#endif

