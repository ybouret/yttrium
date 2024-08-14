
#include "y/chemical/plexus.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Plexus:: ~Plexus() noexcept
        {
        }

        Plexus:: Plexus(const bool verbosity) :
        lib(),
        eqs(),
        wpc( Weasel::Compiler::Instance() ),
        verbose(verbosity),
        xml(verbose),
        ran(),
        cls(0)
        {
        }

        Clusters & Plexus:: assemble()
        {
            return *( cls= new Clusters(eqs,xml) );
        }

        void Plexus:: operator()(Jive::Module  *input)
        {
            wpc(lib,eqs,input);
        }



    }

}
