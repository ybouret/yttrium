
#include "y/chemical/reactive/equilibrium.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Equilibrium:: CallSign = "Equilibrium";
        const char * const Equilibrium:: Label = "Label";
        const char * const Equilibrium:: Reac  = "Reac";
        const char * const Equilibrium:: Prod  = "Prod";


        Equilibrium:: Equilibrium(Library &     lib,
                                  XNode * const eNode,
                                  const size_t  eIndx) :
        Components( Compile::Name(eNode), eIndx )
        {
            Compile::Fill(*this,lib,eNode);
        }

        Equilibrium:: ~Equilibrium() noexcept
        {
        }

        xreal_t Equilibrium:: K(xreal_t t)
        {
            const xreal_t zero;
            const xreal_t value = getK(t);
            if(value<=zero) throw Specific::Exception( key().c_str(), "K(%g)=%g", double(t), double(value));
            return value;
        }

        Equilibrium:: Equilibrium(const String * const s,
                                  const size_t         i) :
        Components(s,i)
        {
        }
        
    }

}


namespace Yttrium
{
    namespace Chemical
    {
       
    }

}



namespace Yttrium
{
    namespace Chemical
    {

      
    }

}

