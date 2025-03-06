
//! \file

#ifndef Y_Chemical_Connected_Included
#define Y_Chemical_Connected_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/fragment.hpp"
#include "y/chemical/type/squad.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        
        //______________________________________________________________________
        //
        //
        //
        //! inner class of connected equilibria/species and topology
        //
        //
        //______________________________________________________________________
        class Connected : public EList, public Fragment, public Squad
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< Chemical::Connected


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Connected(const Equilibrium &first,
                               const unsigned     cindx); //!< initialize
            virtual ~Connected() noexcept;                //!< cleanup
            Connected(const Connected &);                 //!< duplicate

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void connect(const Equilibrium &eq);                //!< add equilibrium and update, atomic
            bool accepts(const Equilibrium &eq) const noexcept; //!< check if sharing species
            void swapAll(Connected &)           noexcept;       //!< swap all
            void replica(const Equilibrium &eq);                //!< add replica equilibrium and update

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Matrix<int>    topology; //!< primary topology
            const unsigned hallmark; //!< within mixes

        private:
            Y_DISABLE_ASSIGN(Connected);
            void fillTopology() noexcept;
        };

    }

}

#endif

