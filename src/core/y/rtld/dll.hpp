//! file

#ifndef Y_RTLD_DLL_INCLUDED
#define Y_RTLD_DLL_INCLUDED 1

#include "y/config/starting.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{

    class DLL
    {
    public:
        static const char * const CallSign; //! "DLL"
        
        explicit DLL(const char *               soname);
        explicit DLL(const Core::String<char> & soname);
        DLL(const DLL &) noexcept;
        virtual ~DLL() noexcept;

        void *symbol(const char *name) noexcept;
        void *symbol(const Core::String<char> &name) noexcept;

    private:
        Y_DISABLE_ASSIGN(DLL);
        class Code;
        Code *code;
    };

}
#endif

