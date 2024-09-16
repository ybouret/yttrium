
//! \file


#ifndef Y_Stream_Proc_Included
#define Y_Stream_Proc_Included 1

#include "y/string.hpp"


namespace Yttrium
{
    class ProcStream
    {
    public:
        virtual ~ProcStream() noexcept;

    protected:
        explicit ProcStream(const char * const cmd,
                            const char * const how);
        void * const handle;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(ProcStream);
    };
}

#endif
