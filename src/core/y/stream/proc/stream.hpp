
//! \file


#ifndef Y_Stream_Proc_Included
#define Y_Stream_Proc_Included 1

#include "y/string.hpp"


namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Process Stream
    //
    //
    //__________________________________________________________________________
    class ProcStream
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        virtual ~ProcStream() noexcept; //!< cleanup

    protected:
        //! open process
        /**
         \param cmd command to execute
         \param how read or write
         */
        explicit ProcStream(const char * const cmd,
                            const char * const how);

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        void * const handle; //!< anonymous system handle


    private:
        Y_DISABLE_COPY_AND_ASSIGN(ProcStream);
    };
}

#endif
