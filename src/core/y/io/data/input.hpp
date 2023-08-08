//! \file

#ifndef Y_IO_Data_Input_Included
#define Y_IO_Data_Input_Included 1

#include "y/io/stream/input.hpp"

namespace Yttrium
{
    namespace Memory { class ReadOnlyBuffer; }

    //______________________________________________________________________
    //
    //
    //
    //! Stream from PERSISTENT memory data
    //
    //
    //______________________________________________________________________
    class InputDataStream : public InputStream
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const CallSign; //!< "InputDataStream"

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit InputDataStream(const void *addr, const size_t size) noexcept; //!< open from user's data
        explicit InputDataStream(const Memory::ReadOnlyBuffer &)      noexcept; //!< open from buffer
        virtual ~InputDataStream()                                    noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual const char * callSign() const noexcept; //!< "CallSigns
        virtual bool query(char     &);                 //!< query
        virtual void store(const char);                 //!< store
        virtual bool ready();                           //!< read

    private:
        Y_DISABLE_COPY_AND_ASSIGN(InputDataStream);
        const char *       curr;
        const char * const last;
        const char * const base;
    };

}

#endif

