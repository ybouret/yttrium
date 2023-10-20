
//! \file

#ifndef Y_Chemical_Reactive_Group_Included
#define Y_Chemical_Reactive_Group_Included 1

#include "y/chem/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Allocatable List of Equilibria
        //
        //______________________________________________________________________
        class EqGroup : public Object, public EqRepo
        {
        public:
            //__________________________________________________________________
            //
            // Definitions
            //__________________________________________________________________
            typedef CxxListOf<EqGroup> List; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit EqGroup(const Equilibrium &first); //!< setup
            virtual ~EqGroup() noexcept;                //!< cleanup
            EqGroup(const EqGroup &);                   //!< copy

            //__________________________________________________________________
            //
            // Members
            //__________________________________________________________________
            EqGroup *next; //!< for list
            EqGroup *prev; //!< for list

        private:
            Y_DISABLE_ASSIGN(EqGroup);
        };


        //______________________________________________________________________
        //
        //
        //! Allocatable List of Groups of Equilibria
        //
        //______________________________________________________________________
        class EqGroups : public Object, public EqGroup::List
        {
        public:
            explicit EqGroups() noexcept; //!< setup
            virtual ~EqGroups() noexcept; //!< cleanup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(EqGroups);
        };

    }

}

#endif

