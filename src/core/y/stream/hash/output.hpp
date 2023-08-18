
//! \file

#ifndef Y_Stream_Hash_Output_Included
#define Y_Stream_Hash_Output_Included 1

#include "y/stream/output.hpp"

namespace Yttrium
{

    namespace Hashing { class Function; }

    //__________________________________________________________________________
    //
    //
    //
    //! Running Hash Function on outpu
    //
    //
    //__________________________________________________________________________
    class HashingStream : public OutputStream
    {
    public:
        static const char * const CallSign; //!< "HashingStream"

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit HashingStream(Hashing::Function &) noexcept; //!< setup from PERSISTENT Hash Function
        virtual ~HashingStream()                    noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual  void        write(const char) noexcept; //!< send to hash
        virtual  void        flush()           noexcept; //!< do nothing
        virtual  const char *callSign()  const noexcept; //!< CallSign

    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashingStream);
        Hashing::Function & hfn;
    };

}

#endif
