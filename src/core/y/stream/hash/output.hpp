
//! \file

#ifndef Y_Stream_Hash_Output_Included
#define Y_Stream_Hash_Output_Included 1

#include "y/stream/output.hpp"

namespace Yttrium
{

    namespace Hashing { class Function; }

    class HashingStream : public OutputStream
    {
    public:
        static const char * const CallSign;

        explicit HashingStream(Hashing::Function &) noexcept;
        virtual ~HashingStream() noexcept;
        virtual  void        write(const char) noexcept;
        virtual  void        flush()           noexcept;
        virtual  const char *callSign()  const noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashingStream);
        Hashing::Function & hfn;
    };

}

#endif
