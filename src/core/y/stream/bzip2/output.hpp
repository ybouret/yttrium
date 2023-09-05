

//! \file

#ifndef Y_Stream_Bzip2_OutputFile_Included
#define Y_Stream_Bzip2_OutputFile_Included 1

#include "y/stream/output.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{
    namespace Bzip2
    {
        //______________________________________________________________________
        //
        //
        //
        //! BZ2 basic output file
        //
        //
        //______________________________________________________________________
        class OutputFile : public OutputStream
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< BZ2::OutputFile

            //______________________________________________________________________
            //
            //
            // C++
            //
            //______________________________________________________________________
            virtual ~OutputFile() noexcept;                                            //!< flush/close
            explicit OutputFile(const char *               fileName, const int level); //!< open with level of compression
            explicit OutputFile(const Core::String<char> & filename, const int level); //!< open with level of compression

            //______________________________________________________________________
            //
            //
            // Methods
            //
            //______________________________________________________________________
            virtual const char * callSign() const noexcept; //!< CallSign
            virtual void         write(const char);         //!< compress one more cahr
            virtual void         flush();                   //!< do nothing, see Bzip2 AI
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputFile);
            class Code;
            Code *code;
        };
    }
}

#endif

