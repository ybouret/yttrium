
//! \file

#ifndef Y_IO_GZIP_Output_File_Imcluded
#define Y_IO_GZIP_Output_File_Imcluded 1

#include "y/stream/output.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{
    
    namespace GZip
    {
        //______________________________________________________________________
        //
        //
        //
        //! Input gzFile
        //
        //
        //______________________________________________________________________
        class OutputFile : public OutputStream
        {
        public:
            //__________________________________________________________________
            //
            //
            // definition
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "GZip::InputFile"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit OutputFile(const char *fileName, const int level, const bool append=false);               //!< open file
            explicit OutputFile(const Core::String<char> &fileName, const int level, const bool append=false); //!< open file
            virtual ~OutputFile() noexcept;                                                   //!< cleanup


            virtual const char * callSign() const noexcept; //!< CallSign
            virtual void         write(const char c);       //!< gzwrite
            virtual void         flush() noexcept;          //!< do nothing here, let gzip run...

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputFile);
            class Code;
            Code *code;
        };

    }

}

#endif

