
//! \file

#ifndef Y_CStr_Ptr_Included
#define Y_CStr_Ptr_Included 1

#include "y/config/starting.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{

    class CStrPtr
    {
    public:
        CStrPtr(const char *);
        CStrPtr(const char *, const char *);
        CStrPtr(const CStrPtr &) noexcept;
        virtual ~CStrPtr() noexcept;
        Y_OSTREAM_PROTO(CStrPtr);

        const char * operator()(void) const noexcept;
        

    private:
        Y_DISABLE_ASSIGN(CStrPtr);
        class  Code;
        Code  *code;
    };

}

#endif
