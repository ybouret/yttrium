
//! \file

#ifndef Y_CStr_Ptr_Included
#define Y_CStr_Ptr_Included 1

#include "y/config/starting.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! helper to produce legacy C strings
    //
    //__________________________________________________________________________
    class CStrPtr
    {
    public:
        CStrPtr(const char *);                        //!< strcpy
        CStrPtr(const char *, const char *);          //!< strcat
        CStrPtr(const CStrPtr &) noexcept;            //!< no-throw copy
        virtual ~CStrPtr() noexcept;                  //!< cleanup
        Y_OSTREAM_PROTO(CStrPtr);                     //!< display
        const char * operator()(void) const noexcept; //!< R/O access
        

    private:
        Y_DISABLE_ASSIGN(CStrPtr);
        class  Code;
        Code  *code;
    };

}

#endif
