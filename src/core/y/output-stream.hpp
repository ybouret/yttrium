
//! \file

#ifndef Y_OUTPUT_STREAM_DECLARE_INCLUDED
#define Y_OUTPUT_STREAM_DECLARE_INCLUDED 1


namespace Yttrium
{
    class OutputStream;
}

//! helper to define an output operator
#define Y_OUTPUT_STREAM(CLASS) friend OutputStream & operator<<(OutputStream &, const CLASS &)

#endif

