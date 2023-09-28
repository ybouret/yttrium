//! file

#ifndef Y_RTLD_DLL_INCLUDED
#define Y_RTLD_DLL_INCLUDED 1

#include "y/config/starting.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Dynamically Loaded Library
    //
    //
    //__________________________________________________________________________
    class DLL
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const CallSign; //!< "DLL"
        
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit DLL(const char *               soname); //!< open library
        explicit DLL(const Core::String<char> & soname); //!< open library
        DLL(const DLL &) noexcept;                       //!< share code
        virtual ~DLL()   noexcept;                       //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        void *symbol(const char *               name) noexcept; //!< load symbol
        void *symbol(const Core::String<char> & name) noexcept; //!< load symbol

    private:
        Y_DISABLE_ASSIGN(DLL);
        class Code;
        Code *code;
    };

}
#endif

