
//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chem/reactive/component.hpp"
#include "y/chem/species/actors.hpp"
#include "y/associative/address-book.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    class AddressBook;

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
            static const char * const                  CallSign;       //!< Components
            typedef Component::DataBase::ConstIterator ConstIterator;  //!< alias

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

            //! fill one line of topology
            template <typename T> inline
            void fill(Writable<T> &nu, const Level level) const
            {
                for(ConstIterator it=db.begin();it!=db.end();++it)
                {
                    const Component &cc = *it;
                    nu[ cc.sp.indx[level] ] = cc.nu;
                }
            }

            //! check if sharing a species with other
            bool linkedTo(const Components &other) const noexcept;

            //! insert species in book
            void insertSpeciesIn(AddressBook &book) const;


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
            virtual ConstInterface & surrogate() const noexcept;

        };

    }

}

#endif


