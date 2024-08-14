
//! \file

#ifndef Y_Chemical_Plexus_Included
#define Y_Chemical_Plexus_Included 1

#include "y/chemical/weasel/compiler.hpp"
#include "y/chemical/plexus/clusters.hpp"
#include "y/random/park-miller.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Plexus
        {
        public:
            explicit Plexus(const bool verbosity=false);
            virtual ~Plexus() noexcept;

            Clusters & assemble();

            void operator()(Jive::Module  *input);

            Library            lib; //!< library
            LuaEquilibria      eqs; //!< equilibria
            Weasel::Compiler  &wpc; //!< Weasel Plexus Compiler
            bool               verbose;
            XMLog              xml;
            Random::ParkMiller ran;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Plexus);
            AutoPtr<Clusters> cls;
        };
    }

}

#endif

