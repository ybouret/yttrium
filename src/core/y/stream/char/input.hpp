//! \file

#ifndef Y_Stream_Char_Input_Included
#define Y_Stream_Char_Input_Included 1

#include "y/stream/input.hpp"

namespace Yttrium
{
    //______________________________________________________________________
    //
    //
    //
    //! Stream from PERSISTENT memory data
    //
    //
    //______________________________________________________________________
    class InputCharStream : public InputStream
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const CallSign; //!< "InputCharStream"

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit InputCharStream(IO::Chars &chars) noexcept; //!< setup by stealing chars
        virtual ~InputCharStream()                 noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual const char * callSign() const noexcept;
        virtual bool         ready()          noexcept;
        virtual bool         query(char &C)  noexcept;
        virtual void         store(const char C);
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(InputCharStream);
        IO::Chars cache;
    };
}

#endif

