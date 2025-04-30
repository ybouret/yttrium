//! \file

#ifndef Y_Chemical_Plexus_Included
#define Y_Chemical_Plexus_Included 1

#include "y/chemical/plexus/equalizer.hpp"
#include "y/chemical/plexus/reactors.hpp"
#include "y/chemical/plexus/initial/design.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Plexus
        {
        public:
            static const char * const CallSign;

            explicit Plexus(XMLog         &  xml,
                            const Library & _lib,
                            Equilibria    & _eqs,
                            const xreal_t   t0);

            virtual ~Plexus() noexcept;


            void operator()(XMLog     &xml,
                            XWritable &C0,
                            Reactor::Proc * const cb=0);


            const Library    & lib;
            const Equilibria & eqs;
            Clusters           cls;
            Equalizer          eqz;
            Reactors           rxn;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Plexus);
        };

    }
}

#endif

