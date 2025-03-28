
//! \file

#ifndef Y_Chemical_Company_Included
#define Y_Chemical_Company_Included 1

#include "y/chemical/reactive/equilibrium/actor.hpp"

namespace Yttrium
{
    namespace Chemical
    {

    
        //______________________________________________________________________
        //
        //
        //
        //! List of actors
        //
        //
        //______________________________________________________________________
        class Company : public Entity, public Actor::List
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Company(const NameForm) noexcept; //!< setup empty
            virtual ~Company() noexcept; //!< cleanup
            Company(const Company &);    //!< duplicate

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const String & key() const noexcept; //!< [Entity] name

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Actor & recruit(Actor * const);                    //!< recruit and update name
            bool          lookFor(const Species &)   const noexcept; //!< look for species
            void          xch(Company &)                   noexcept; //!< exchange all
            String        html()                              const; //!< make html label with form
            bool          worksWith(const Company &) const noexcept; //!< sharing species

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const String     name; //!< pre-computed
            const NameForm   form; //!< name form

        private:
            Y_DISABLE_ASSIGN(Company);
        };

    }

}

#endif

