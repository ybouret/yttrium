
//! \file

#ifndef Y_IO_Libc_Input_Included
#define Y_IO_Libc_Input_Included 1

#include "y/io/stream/input.hpp"
#include "y/io/libc/file.hpp"
#include "y/io/libc/buffer/cached.hpp"

namespace Yttrium
{
    namespace Core { template <typename> class String; }


    //__________________________________________________________________________
    //
    //! Hard-coded file name redirecting to stdin
    //__________________________________________________________________________
#define Y_STDIN ":STDIN:"

    namespace Libc
    {
        //______________________________________________________________________
        //
        //
        //! analyze file name of assuming stdin
        //
        //______________________________________________________________________
        class IsStdIn
        {
        public:
            virtual ~IsStdIn() noexcept;    //!< cleanup

        protected:
            explicit IsStdIn() noexcept;    //!< flag = true
            explicit IsStdIn(const char *); //!< check valid file name and stdin
            const bool flag;                //!< true if stdin

        private:
            Y_DISABLE_COPY_AND_ASSIGN(IsStdIn);
        };

        //______________________________________________________________________
        //
        //
        //! InputStream based on legacy FILE
        //
        //______________________________________________________________________
        class InputFile : public InputStream, public IsStdIn, public File
        {
        public:
            static const char * const CallSign;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~InputFile() noexcept;                           //!< cleanup
            explicit InputFile(const StdIn_ &);                      //!< stdin
            explicit InputFile(const char *fileName);                //!< regular/stdin
            explicit InputFile(const Core::String<char> &fileName);  //!< regilar/stdin

            //__________________________________________________________________
            //
            //
            // interface
            //
            //__________________________________________________________________
            virtual bool query(char &);     //!< query new char
            virtual void store(const char); //!< store read char
            virtual bool ready();           //!< char in buffer or try to read
            
            virtual const char *callSign() const noexcept; 

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void         gc()     noexcept; //!< prune buffer
            bool         gets(IO::Chars &line); //!< get a line delimited by cr,lf, or crlf

        private:
            Y_DISABLE_COPY_AND_ASSIGN(InputFile);
            Libc::CachedBuffer buffer;
        };
    }

}

#endif

