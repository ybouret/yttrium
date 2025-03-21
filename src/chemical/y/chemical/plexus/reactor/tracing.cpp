
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
            tracing.free();
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

        void Reactor:: EmitGnuPlotTracing(std::ostream &os, const String &baseName)  
        {
            os << "'" << baseName << '.' << ProfileExt << "' u 1:2 w l";
        }

        void Reactor:: emitGnuPlotTracing(std::ostream &os) const
        {
            os << "# Gnuplot Tracing" << std::endl;
            if(tracing.size()>0)
            {
                EmitGnuPlotTracing(os << "plot ",tracing[1]);
                for(size_t i=2;i<=tracing.size();++i)
                    EmitGnuPlotTracing(os << ", ",tracing[i]);
                os << std::endl;
            }
            else
            {
                os << "# no tracing!" << std::endl;
            }
        }

    }

}

