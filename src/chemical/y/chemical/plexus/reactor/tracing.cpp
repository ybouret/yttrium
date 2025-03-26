
#include "y/chemical/plexus/reactor.hpp"
#include "y/stream/libc/output.hpp"
#include "y/lingo/vfs/find.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        using namespace Lingo;

        bool               Reactor:: EmitProfiles = false;
        const char * const Reactor:: ProfileExt   = "pro";

        void Reactor:: eraseOlderProfiles() noexcept
        {
            profiles.free();
            LocalFileSystem::TryRemoveExtension::In(".",ProfileExt);
        }

        void Reactor:: EraseIfDifferentOf(const String &baseName)
        {
            VFS &        vfs = LocalFileSystem::Get();
            VFS::Entries entries;

            VirtualFileSystem::Find(vfs,entries, ".", ProfileExt, Lingo::Matching::Exactly,VFS::Entry::Ext);

            const String target = baseName + '.' + ProfileExt;
            bool         found  = false;
            for(VFS::Entry *ep=entries.head;ep;ep=ep->next)
            {
                if(ep->base == target)
                {
                    found = true;
                    continue;
                }
                vfs.tryRemoveFile(ep->path);
            }
            if(!found)
                throw Specific::Exception(CallSign,"Missing '%s'", target.c_str());

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
            os << "# Gnuplot Profiles" << std::endl;
            if(profiles.size()>0)
            {
                EmitGnuPlotTracing(os << "plot ",profiles[1]);
                for(size_t i=2;i<=profiles.size();++i)
                    EmitGnuPlotTracing(os << ", ",profiles[i]);
                os << std::endl;
            }
            else
            {
                os << "# no profiles!" << std::endl;
            }
        }

    }

}

