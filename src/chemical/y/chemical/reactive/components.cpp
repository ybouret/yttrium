

#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Components:: ~Components() noexcept
        {}


        

        void Components:: operator()(const int      nu,
                                     const Species &sp)
        {

            Actors *ac = 0;
            switch( Sign::Of(nu) )
            {
                case 0: throw Specific::Exception(name.c_str(), "zero stoichiometry for '%s'", sp.name.c_str());
            }

        }



    }

}


