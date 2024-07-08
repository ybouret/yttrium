
#include "y/chemical/reactive/aftermath.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Aftermath:: ~Aftermath() noexcept
        {
        }

        Aftermath:: Aftermath()  : xmul()
        {
        }

        bool Aftermath:: solve(XWritable       &Cout,
                               const Level      out,
                               const XReadable &Cinp,
                               const Level      inp,
                               const Components &E,
                               const xreal_t     K)
        {
            switch(E.kind)
            {
                case Components::Nebulous:
                    break;

                case Components::ProdOnly:
                    if( E.prod.wouldRun(Cinp,inp) )
                    {
                        return true;
                    }
                    break;

                case Components::ReacOnly:
                    if( E.reac.wouldRun(Cinp,inp) )
                    {
                        return true;
                    }
                    break;

                case Components::Standard:
                    
                    break;
            }
            return false;
        }

    }

}

