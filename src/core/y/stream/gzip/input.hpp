
//! \file

#ifndef Y_IO_GZIP_Input_File_Imcluded
#define Y_IO_GZIP_Input_File_Imcluded 1

#include "y/stream/input.hpp"

namespace Yttrium
{
    namespace Core { template <typename> class String; }

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
        class InputFile : public InputStream
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
            explicit InputFile(const char *fileName);               //!< open file
            explicit InputFile(const Core::String<char> &fileName); //!< open file
            virtual ~InputFile() noexcept;                          //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept; //!< [Identifiable] CallSign
            virtual bool         query(char &);             //!< [InputStream] return next char
            virtual void         store(const char);         //!< [InputStream] unread char
            virtual bool         ready();                   //!< [InputStream] check status

        private:
            Y_DISABLE_COPY_AND_ASSIGN(InputFile);
            class Code;
            Code *code;
        };

    }

}

#endif
