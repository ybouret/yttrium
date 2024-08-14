
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

        //______________________________________________________________________
        //
        //
        //
        //! ready chemical system
        //
        //
        //______________________________________________________________________
        class Plexus
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Plexus(const bool verbosity=false); //!< initialize
            virtual ~Plexus() noexcept;                  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Clusters & assemble();           //!< create clusters
            void operator()(Jive::Module *); //!< compile data

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Library            lib;      //!< library
            LuaEquilibria      eqs;      //!< equilibria
            Weasel::Compiler  &wpc;      //!< Weasel Plexus Compiler
            bool               verbose; //!< verbosity
            XMLog              xml;     //!< verbose helper
            Random::ParkMiller ran;     //!< built-in random

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Plexus);
            AutoPtr<Clusters> cls;
        };
    }

}

#endif

