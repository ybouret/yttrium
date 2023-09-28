
//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chem/component.hpp"
#include "y/chem/actors.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Components: component as reactants and products
        //
        //     
        //______________________________________________________________________
        class Components : public Proxy<const Component::DataBase>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< components

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Components();            //!< setup
            virtual ~Components() noexcept;   //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void operator()(const int nu, const Species &sp); //!< add nu * sp
            String toString() const;                          //!< convert to string

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Actors reac; //!< reactants
            const Actors prod; //!< products

        private:
            Y_DISABLE_ASSIGN(Components);
            Component::DataBase db;
            virtual ConstType & surrogate() const noexcept;

        };

    }

}

#endif


