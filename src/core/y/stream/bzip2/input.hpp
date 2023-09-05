//! \file

#ifndef Y_Stream_Bzip2_InputFile_Included
#define Y_Stream_Bzip2_InputFile_Included 1

#include "y/stream/input.hpp"

namespace Yttrium
{
    
    namespace Bzip2
    {

        //______________________________________________________________________
        //
        //
        //
        //! reading Bzip2 compressed file
        //
        //
        //______________________________________________________________________
        class InputFile : public InputStream
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char  * const CallSign; //!< Bzip2::InputFile

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit InputFile(const char *               fileName); //!< open filename
            explicit InputFile(const Core::String<char> & fileName); //!< open filename
            virtual ~InputFile() noexcept;                           //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept; //!< CallSign
            virtual bool query(char &C);
            virtual void store(char C);
            virtual bool ready();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(InputFile);
            class Code;
            Code *code;

        };
    }
}

#endif

