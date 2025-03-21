
#include "y/chemical/plexus/reactor.hpp"
#include "y/stream/libc/output.hpp"
#include "y/lingo/vfs/find.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        bool Reactor::Trace = false;

        const char * const Reactor:: ProfileExt = "pro";

        void Reactor:: eraseOlderProfiles() noexcept
        {
            Lingo::LocalFileSystem::TryRemoveExtension::In(".",ProfileExt);
        }


        void Reactor:: saveCurrentProfile(const String &baseName, const size_t np)
        {
            const String fn = baseName + '.' + ProfileExt;
            OutputFile   fp(fn);
            for(size_t i=0;i<=np;++i)
            {
                const real_t u = real_t(i)/np;
                const real_t f = real_t((*this)(u));
                fp("%.15g %.15g\n",u,f);
            }
        }
    }

}

