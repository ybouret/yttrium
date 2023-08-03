
//! \file

#ifndef Y_CStr_Ptr_Included
#define Y_CStr_Ptr_Included 1

#include "y/container/readable.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! helper to produce legacy C strings
    //
    //__________________________________________________________________________
    class CStrPtr : public Readable<char>
    {
    public:
        static const char * const CallSign;           //!< "CStrPtr"
        CStrPtr(const char *);                        //!< strcpy
        CStrPtr(const char *, const char *);          //!< strcat
        CStrPtr(const CStrPtr &) noexcept;            //!< no-throw copy
        virtual ~CStrPtr() noexcept;                  //!< cleanup
        Y_OSTREAM_PROTO(CStrPtr);                     //!< display

        virtual size_t       size()                    const noexcept; //!< [Collection]  strlen
        virtual const char * callSign()                const noexcept; //!< [Identfiable] CallSign;
        virtual const char  &operator[](const size_t ) const noexcept; //!< [Readable]    access
        const char          *operator()(void)          const noexcept; //!< access c-string
        
    private:
        Y_DISABLE_ASSIGN(CStrPtr);
        class  Code;
        Code  *code;
    };

}

#endif
