
#include "y/chemical/reactive/equilibrium/components.hpp"
#include "y/system/exception.hpp"
#include <cerrno>
#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Components::Scheme = "set18";

        String Components:: makeColor() const
        {
            return makeColorFrom(Scheme);
        }

        void Components :: viz(OutputStream &fp,
                               const char * const color,
                               const char * const style) const
        {
            Node(fp,this) << '[';
            Label(fp,name) << ",shape=box3d";
            if(color) fp << ",color=" << color << ",fontcolor=" << color;
            if(style) fp << "style=\"" << style << "\"";
            Endl(fp<<']');

            for(ConstIterator it=cmdb.begin();it!=cmdb.end();++it)
            {
                const Component &cm = *it;
                cm.viz(fp,*this,color);
            }

        }

        void Components:: graphViz(OutputStream &fp) const
        {
            Enter(fp, "G");
            for(ConstIterator it=cmdb.begin();it!=cmdb.end();++it)
            {
                const Component &cm = *it;
                cm.actor.sp.viz(fp,0,0);
            }
            viz(fp,0,0);
            Leave(fp);
        }

        
    }

}

