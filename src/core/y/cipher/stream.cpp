
#include "y/cipher/stream.hpp"

namespace Yttrium
{
    StreamCipher:: StreamCipher() noexcept :
    Cipher(),
    InputStream(),
    OutputStream()
    {
    }

    StreamCipher:: ~StreamCipher() noexcept
    {
    }

    
}
