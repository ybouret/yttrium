
//! \file


#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1


#include "y/chemical/reactive/component.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! database of components
        //
        //
        //______________________________________________________________________
        typedef SuffixSet<const String,const Component> ComponentsDB;

        //______________________________________________________________________
        //
        //
        //
        //! Latchable Database of components
        //
        //
        //______________________________________________________________________
        class ComponentsType : public ComponentsDB, public Latchable
        {
        public:
            explicit ComponentsType();           //!< setup
            virtual ~ComponentsType() noexcept;  //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ComponentsType);
        };

#if 0
        enum ComponentsTier
        {
            Deserted, //!< empty...
            Standard, //!< at least one conserved species on each side
            ProdOnly, //!< only (unbounded) product(s)
            ReacOnly, //!< only (unboudned) reactant(s)
            Nebulous  //!< reac(s) and prod(s), all unbounded
        };
#endif

        //______________________________________________________________________
        //
        //
        //
        //! named components
        //
        //
        //______________________________________________________________________
        class Components : public Indexed, public Proxy<const ComponentsType>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const             Symbol;          //!< "<=>";
            static const char                     Separator = ':'; //!< for parser
            typedef ComponentsType::ConstIterator ConstIterator;   //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Components(const String * const, const size_t); //!< setup
            virtual ~Components() noexcept;                          //!< cleanup
            Y_OSTREAM_PROTO(Components);                             //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void use(const Role, const unsigned, const Species &); //!< use new actor
            void use(const Role, const  Species &);                //!< use(role,1,species)
            void latch() noexcept;                                 //!< latch all

            //! check connected by species
            static bool AreConnected(const Components &lhs, const Components &rhs);

            //! fill compatible topology
            template <typename ARRAY> inline
            void fillTopology(ARRAY &topo, const Level level) const
            {
                typedef typename ARRAY::Type _T;
                for(const Actor *a=prod->head;a;a=a->next) a->sp(topo,level) =  _T(a->nu);
                for(const Actor *a=reac->head;a;a=a->next) a->sp(topo,level) = -_T(a->nu);
            }


            void gatherSpeciesIn(AddressBook &book) const; //!< OR'ed species into bool
            bool neutral()                 const noexcept; //!< check neutrality

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            const Actors reac; //!< current reactants
            const Actors prod; //!< current products

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Components);
            Y_PROXY_DECL();
        protected:
            ComponentsType db; //!< database
        };

    }

}

#endif

