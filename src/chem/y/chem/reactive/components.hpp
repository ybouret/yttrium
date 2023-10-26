
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

            template <typename ARRAY, typename PROC> inline
            std::ostream &display(std::ostream &os, ARRAY &arr, const Level lvl, PROC proc) const
            {
                os << '{';

                for(const Actor *a=reac.head;a;a=a->next)
                {
                    os << ' ' << a->sp << '=' << proc(arr[a->sp.indx[lvl]]);
                }

                for(const Actor *a=prod.head;a;a=a->next)
                {
                    os << ' ' << a->sp << '=' << proc(arr[a->sp.indx[lvl]]);
                }

                os << ' ' << '}';
                return os;
            }

            template <typename ARRAY> inline
            std::ostream &display(std::ostream &os, ARRAY &arr, const Level lvl) const
            {
                return display(os,arr,lvl,Conv::Id<typename ARRAY::MutableType>);
            }


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

        };

    }

}

#endif


