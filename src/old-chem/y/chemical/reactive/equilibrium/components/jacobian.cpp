#include "y/chemical/reactive/equilibrium/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Components:: jacobian(XWritable &      phi, const Level J,
                                  const XReadable &C,   const Level L) const
        {
            {
                const xReal _0;
                phi.ld(_0);
            }

            ConstIterator it=cmdb.begin();
            for(size_t i=cmdb.size();i>0;--i,++it)
            {
                const Component &cm = *it;
                const Actor     &ac  = cm.actor;
                const Species   &sp  = ac.sp;
                switch(cm.role)
                {
                    case Product:  sp(phi,J) =  ac.xn/sp(C,L); break;
                    case Reactant: sp(phi,J) = -ac.xn/sp(C,L); break;
                }
            }
        }
    }

}
