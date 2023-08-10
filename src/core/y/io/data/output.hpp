
//! \file

#ifndef Y_IO_Data_Output_Included
#define Y_IO_Data_Output_Included 1

#include "y/io/stream/output.hpp"


namespace Yttrium
{

    namespace Core
    {
        class OutputDataStream : public OutputStream
        {
        public:
            static const char * const CallSign;
            explicit OutputDataStream() noexcept;
            virtual ~OutputDataStream() noexcept;
            virtual const char *callSign() const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputDataStream);
        };
    }

    template <typename SEQUENCE>
    class OutputDataStream : public Core::OutputDataStream, public SEQUENCE
    {
    public:
        inline explicit OutputDataStream() : Core::OutputDataStream(), SEQUENCE()
        {
        }

        inline virtual ~OutputDataStream() noexcept
        {
        }

        inline virtual void write(const char c)
        {
            SEQUENCE &self = *this;
            self << c;
        }

        inline virtual void flush()  {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(OutputDataStream);
    };


}

#endif
