
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

            //! display compact values from array using species level
            template <typename ARRAY, typename PROC> inline
            std::ostream &display(std::ostream &os, ARRAY &arr, const Level lvl, PROC proc) const
            {
                os << '{';
                display(os,reac,arr,lvl,proc);
                display(os,prod,arr,lvl,proc);
                os << ' ' << '}';
                return os;
            }

            //! default display
            template <typename ARRAY> inline
            std::ostream &display(std::ostream &os, ARRAY &arr, const Level lvl) const
            {
                return display(os,arr,lvl,Conv::Id<typename ARRAY::MutableType>);
            }

            //! transfert source to target using species levels
            template <typename TARGET,typename SOURCE>
            inline void transfer(TARGET &target, const Level targetLevel,
                                 SOURCE &source, const Level sourceLevel) const
            {
                size_t        n = db.size();
                ConstIterator i = db.begin();
                while(n-- > 0)
                {
                    const Species   &sp = (*i).sp;
                    target[ sp.indx[targetLevel] ] = source[ sp.indx[sourceLevel] ];
                    ++i;
                }
            }

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

            template <typename ARRAY, typename PROC> inline
            static void display(std::ostream &os, const Actors &actors, ARRAY &arr, const Level lvl, PROC proc)
            {
                for(const Actor *a=actors.head;a;a=a->next)
                    os << ' ' << a->sp << '=' << proc(arr[a->sp.indx[lvl]]);
            }
        };

    }
    
}

#endif


