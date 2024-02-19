

//! \file

#ifndef Y_STREAM_DATA_OUTPUT_BUFFER_INCLUDED
#define Y_STREAM_DATA_OUTPUT_BUFFER_INCLUDED 1

#include "y/stream/data/buffer.hpp"
#include "y/stream/data/output.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Output stream into a fixed-sized buffer
    //
    //
    //__________________________________________________________________________
    template <size_t N>
    class OutputBuffer : public DinkyBuffer<N>, public OutputDataStream< DinkyBuffer<N> >
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef DinkyBuffer<N>               BufferType; //!< alias
        typedef OutputDataStream<BufferType> OutputType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        inline explicit OutputBuffer() noexcept : BufferType(), OutputType( static_cast<BufferType&>(*this) ) {} //!< setup
        inline virtual ~OutputBuffer() noexcept {}                                                               //!< cleanup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(OutputBuffer);
    };
}

#endif

