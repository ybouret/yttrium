
//! \file

#ifndef Y_Chemical_Connected_Included
#define Y_Chemical_Connected_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //! inner class of connected equilibria/species and topology
        class Connected : public EList, public Fragment
        {
        public:
            static const char * const CallSign;

            explicit Connected(const Equilibrium &first);
            virtual ~Connected() noexcept;
            Connected(const Connected &);

            void connect(const Equilibrium &eq);
            bool accepts(const Equilibrium &eq) const noexcept;
            void swapAll(Connected &) noexcept;

            SList       species;
            Assembly    sformat;  //!< for species
            Matrix<int> topology; //!< primary topology

        private:
            Y_DISABLE_ASSIGN(Connected);
            void fillTopology() noexcept;
        };

    }

}

#endif

