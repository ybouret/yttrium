#include "y/chemical/plexus.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Plexus:: CallSign = "Plexus";

        static inline
        const Library & PlexusCheck(const Library &lib)
        {
            if( !lib.latched ) throw Specific::Exception(Plexus::CallSign, "%s must be latched", Library::CallSign);
            return lib;
        }

        static inline
        Equilibria & PlexusCheck(Equilibria &eqs)
        {
            if( eqs.latched ) throw Specific::Exception(Plexus::CallSign, "%s must be unlatched", Equilibria::CallSign);
            return eqs;
        }


        Plexus:: Plexus(XMLog         &  xml,
                        const Library & _lib,
                        Equilibria    & _eqs,
                        const xreal_t   t0) :
        lib( PlexusCheck(_lib) ),
        eqs( PlexusCheck(_eqs) ),
        cls( xml, _eqs, lib, t0 ),
        eqz(cls),
        rxn(cls)
        {
            
        }

        Plexus:: ~Plexus() noexcept
        {
        }
        
        void Plexus:: operator()(XMLog     &xml,
                                 XWritable &C0,
                                 Reactor::Proc * const cb)
        {
            eqz(xml,C0);
            rxn(xml,C0,cb);
        }

    }

}

