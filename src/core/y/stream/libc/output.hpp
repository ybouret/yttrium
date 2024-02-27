

//! \file

#ifndef Y_IO_Libc_Output_Included
#define Y_IO_Libc_Output_Included 1

#include "y/stream/output.hpp"
#include "y/stream/libc/file.hpp"
#include "y/stream/libc/writable/file.hpp"
#include "y/stream/libc/buffer/direct.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{



    //__________________________________________________________________________
    //
    //
    //
    //! OutputStream based on a C FILE
    //
    //
    //__________________________________________________________________________
    class OutputFile : public OutputStream, public Libc::WritableFile
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definition
        //
        //______________________________________________________________________
        static const char * const CallSign; //!< "OutputFile"

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        virtual ~OutputFile() noexcept;       //!< cleanup
        explicit OutputFile(const StdErr_ &); //!< stderr
        explicit OutputFile(const StdOut_ &); //!< stdout

        //! open regular files of Y_STDERR/Y_STDOUT
        explicit OutputFile(const char *fileName, const bool append=false);

        //! open regular files of Y_STDERR/Y_STDOUT
        explicit OutputFile(const Core::String<char> &fileName, const bool append=false);

        //! overwrite filename
        static void Overwrite(const Core::String<char> & filename);

        //! overwrite filename
        static void Overwrite(const char *               filename);


        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual void         write(const char);        //!< emit/write to buffer
        virtual void         flush();                  //!< emit
        virtual const char * callSign() const throw(); //!< CallSign

    private:
        Y_DISABLE_COPY_AND_ASSIGN(OutputFile);
        Libc::DirectBuffer buffer;
        void emit();
    };


}


#endif
