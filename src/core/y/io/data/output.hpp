
//! \file

#ifndef Y_IO_Data_Output_Included
#define Y_IO_Data_Output_Included 1

#include "y/io/stream/output.hpp"


namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //! base class for OutputDataStream
        //______________________________________________________________________
        class OutputDataStream : public OutputStream
        {
        public:
            static const char * const CallSign;             //!< "OutputDataStream"
            explicit OutputDataStream()          noexcept;  //!< setup
            virtual ~OutputDataStream()          noexcept;  //!< cleanup
            virtual const char *callSign() const noexcept;  //!< CallSign

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputDataStream);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! OutputStream on a SEQUENCE
    //
    //
    //__________________________________________________________________________
    template <typename SEQUENCE>
    class OutputDataStream : public Core::OutputDataStream, public SEQUENCE
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit OutputDataStream() : Core::OutputDataStream(), SEQUENCE() {} //!< setup
        inline virtual ~OutputDataStream() noexcept {}                               //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual void write(const char c) { SEQUENCE &self = *this; self << c; } //!< write by appending
        inline virtual void flush()  {}                                                //!< do nothing

    private:
        Y_DISABLE_COPY_AND_ASSIGN(OutputDataStream);
    };


}

#endif
