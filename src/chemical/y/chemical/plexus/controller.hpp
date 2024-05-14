
//! \file

#ifndef Y_Chemical_Controller_Included
#define Y_Chemical_Controller_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/plexus/fence.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        
        //______________________________________________________________________
        //
        //
        //
        //! equilibrium stripped from its unbounded components
        //
        //
        //______________________________________________________________________
        class Controller : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            typedef Components::ConstIterator ConstIterator; //!< alias

            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from equilibrium
            explicit Controller(const Equilibrium &eq,
                                const AddressBook &conserved);
            
            //! cleanup
            virtual ~Controller() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
           
            //! true if analogous components
            bool isEquivalentTo(const Controller &) const noexcept;
            


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Equilibrium &primary;    //!< persistent primary equilibrium
            const Components   components; //!< conserved components
            Controller        *next;       //!< for list
            Controller        *prev;       //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Controller);
        };

        typedef CxxListOf<Controller> Controllers; //!< list of controllers

    }

}

#endif

