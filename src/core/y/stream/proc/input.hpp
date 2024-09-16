//! \file


#ifndef Y_Stream_Proc_Input_Included
#define Y_Stream_Proc_Input_Included 1

#include "y/stream/proc/stream.hpp"
#include "y/stream/input.hpp"
#include "y/stream/io/chars.hpp"

namespace Yttrium
{
    
    class ProcInput : public ProcStream, public InputStream
    {
    public:
        static const char * const How; //!< "r"
        static const char * const CallSign; //!< "ProcInput"

        explicit ProcInput(const String &     cmd);
        explicit ProcInput(const char * const cmd);
        virtual ~ProcInput() noexcept;

        // [Identifiable]
        virtual const char * callSign() const noexcept;

        // [InputStream]
        virtual bool query(char & data);   //!< query a single char, false = EOS
        virtual void store(const  char) ;  //!< unread a char
        virtual bool ready();              //!< next char ?

        IO::Chars buffer;
    private:
        Y_DISABLE_COPY_AND_ASSIGN(ProcInput);
    };

}

#endif
