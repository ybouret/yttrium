
//! \file

#ifndef Y_IO_Libc_Input_Included
#define Y_IO_Libc_Input_Included 1

#include "y/stream/input.hpp"
#include "y/stream/libc/readable/file.hpp"
#include "y/stream/libc/file.hpp"
#include "y/stream/libc/buffer/cached.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{



    //__________________________________________________________________________
    //
    //
    //! InputStream based on legacy FILE
    //
    //__________________________________________________________________________
    class InputFile : public InputStream, public Libc::ReadableFile
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const CallSign; //!< "InputFile"

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        virtual ~InputFile() noexcept;                           //!< cleanup
        explicit InputFile(const StdIn_ &);                      //!< stdin
        explicit InputFile(const char *fileName);                //!< regular/stdin
        explicit InputFile(const Core::String<char> &fileName);  //!< regilar/stdin



        //______________________________________________________________________
        //
        //
        // interface
        //
        //______________________________________________________________________
        virtual bool query(char &);     //!< query new char
        virtual void store(const char); //!< store read char
        virtual bool ready();           //!< char in buffer or try to read

        virtual const char *callSign() const noexcept;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        void         gc()     noexcept; //!< prune buffer

    private:
        Y_DISABLE_COPY_AND_ASSIGN(InputFile);
        Libc::CachedBuffer buffer;
    };


}

#endif

