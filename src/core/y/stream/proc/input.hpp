//! \file


#ifndef Y_Stream_Proc_Input_Included
#define Y_Stream_Proc_Input_Included 1

#include "y/stream/proc/stream.hpp"
#include "y/stream/input.hpp"
#include "y/stream/io/chars.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Input Process Stream
    //
    //
    //__________________________________________________________________________
    class ProcInput : public ProcStream, public InputStream
    {
    public:
        static const char * const How;      //!< "r"
        static const char * const CallSign; //!< "ProcInput"

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit ProcInput(const String &     cmd); //!< start command
        explicit ProcInput(const char * const cmd); //!< start command
        virtual ~ProcInput() noexcept;              //!< cleanup

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________

        // [Identifiable]
        virtual const char * callSign() const noexcept; //!< CallSing

        // [InputStream]
        virtual bool query(char & data);   //!< query a single char, false = EOS
        virtual void store(const  char) ;  //!< unread a char
        virtual bool ready();              //!< next char ?

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! append lines produced by cmd
        template <typename COMMAND>
        static Sequence<String> & AppendTo( Sequence<String> &lines, const COMMAND & cmd)
        {
            ProcInput inp( cmd );
            String    line;
            while(inp.gets(line)) lines << line;
            return lines;
        }

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        IO::Chars buffer; //!< auxiliary I/O buffer to unread chars

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ProcInput);
    };

}

#endif
