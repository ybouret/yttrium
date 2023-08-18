
//! \file

#ifndef Y_Stream_Data_Output_Included
#define Y_Stream_Data_Output_Included 1

#include "y/stream/output.hpp"


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
            virtual void        flush();                    //!< do nothing

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputDataStream);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! OutputStream on a persistent SEQUENCE
    //
    //
    //__________________________________________________________________________
    template <typename SEQUENCE>
    class OutputDataStream : public Core::OutputDataStream
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit OutputDataStream(SEQUENCE &seq) : Core::OutputDataStream(), host(seq) {} //!< setup
        inline virtual ~OutputDataStream() noexcept {}                                           //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual void write(const char c) { host << c; } //!< write by appending

    private:
        Y_DISABLE_COPY_AND_ASSIGN(OutputDataStream);
        SEQUENCE &host;
    };


}

#endif
