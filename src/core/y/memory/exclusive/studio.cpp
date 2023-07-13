
#include "y/memory/exclusive/studio.hpp"
#include "y/system/exception.hpp"
#include <iostream>
#include <cstdlib>

namespace Yttrium
{

    namespace Core
    {
        Studio:: ~Studio() noexcept {}
        Studio:: Studio() noexcept {}

        const char Studio:: MultipleNew[]    = "operator new[]";
        const char Studio:: MultipleDelete[] = "operator delete[]";

        static const char prolog[] = "Unauthorized ";

        void * Studio:: ThrowUnauthorized(const char *clid, const char *fnid)
        {
            assert(0!=clid);
            assert(0!=fnid);
            throw Specific::Exception(clid,"%s%s",prolog,fnid);
        }

        void  Studio:: AbortUnauthorized(const char *clid, const char *fnid) noexcept
        {
            static const char pfx[]= "*** ";
            assert(0!=clid);
            assert(0!=fnid);
            std::cerr << pfx << "for " << clid << std::endl;
            std::cerr << pfx << prolog << fnid << std::endl;
            abort();
        }
    }

}


