//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chemical/reactive/component.hpp"
#include "y/chemical/reactive/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! internal component database
        //
        //
        //______________________________________________________________________
        typedef SuffixSet<const String, const Component> Compendium;

        //______________________________________________________________________
        //
        //
        //
        //! database of components
        //
        //
        //______________________________________________________________________
        class Components : public Entity,  Proxy<const Compendium>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! create empty
            template <typename NAME> inline
            explicit Components(const NAME &_name) :
            Entity(),  Proxy<const Compendium>(),
            name(_name), reac(), prod(), cmdb()
            {
            }

            //! cleanup
            virtual ~Components() noexcept;

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

            //! create new actor with role
            void operator()(const Role role, const unsigned nu, const Species &sp);

            //! create new actor with role and unit coefficient
            void operator()(const Role role, const Species &sp);


            //! quick display
            std::ostream & print(std::ostream &) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const String name; //!< unique name
            const Actors reac; //!< reactants
            const Actors prod; //!< products

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Components);
            virtual ConstInterface & surrogate() const noexcept;
            Compendium cmdb;

            Actors & actorsPlaying(const Role);

        };

    }

}

#endif

