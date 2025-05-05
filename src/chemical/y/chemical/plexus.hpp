//! \file

#ifndef Y_Chemical_Plexus_Included
#define Y_Chemical_Plexus_Included 1

#include "y/chemical/plexus/equalizer.hpp"
#include "y/chemical/plexus/reactors.hpp"
#include "y/chemical/plexus/initial/axioms.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        
        class Plexus
        {
        public:
            static const char * const CallSign; //!< "Plexus"

            explicit Plexus(XMLog         &  xml,
                            const Library & _lib,
                            Equilibria    & _eqs,
                            const xreal_t   t0);

            virtual ~Plexus() noexcept;

            //! solve current solution
            void operator()(XMLog     &           xml,
                            XWritable &           C0,
                            Reactor::Proc * const callback=0);

            //! initialize solution
            void operator()(XMLog     &             xml,
                            XWritable &             C0,
                            const Initial::Axioms & axioms,
                            Reactor::Proc * const   callback=0);

            xreal_t mix(XMLog           &xml,
                        XWritable       &Cm,
                        const XReadable &Ca, const xreal_t Va,
                        const XReadable &Cb, const xreal_t Vb,
                        Reactor::Proc * const callback=0);

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

