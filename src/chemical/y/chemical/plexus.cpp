
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


        static const char * const UserData = "User Data";

        void Plexus:: operator()(const char * const input)
        {
            Plexus &self = *this;
            self( Jive::Module::OpenData(UserData,input) );
        }

        void Plexus:: operator()(const String &input)
        {
            Plexus &self = *this;
            self( Jive::Module::OpenData(UserData,input) );
        }


    }

}
