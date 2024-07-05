
//! \file

#ifndef Y_Chemical_Conservation_Group_Included
#define Y_Chemical_Conservation_Group_Included 1

#include "y/chemical/reactive/conservation/law.hpp"
#include "y/chemical/type/entities.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            typedef Small::BareLightList<const Law> LawList; //!< alias
            typedef LawList::NodeType               LawNode; //!< alias

            //__________________________________________________________________
            //
            //
            //
            //! Group of dependent conservation laws
            //
            //
            //__________________________________________________________________
            class Group : public Object, public Entities, public Proxy<const LawList>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<Group>              List;  //!< alias
                typedef Vector<size_t,Memory::Dyadic> Table; //!< table to address

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Group(const Law &first); //!< setup with first law
                virtual ~Group() noexcept;        //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                bool accepts(const Law &)   const noexcept; //!< true is sharing at least one species
                bool accepts(const Group &) const noexcept; //!< true is sharing at least one species
                void append(const Law &);                   //!< append a new law
                void append(Group &) noexcept;              //!< steal new group content

                //! compile species and law
                /**
                 - gather all species
                 - make their aux level
                 - make the topLevel table
                 - make algebraic laws
                 */
                void compile();

                //std::ostream & padLaw(std::ostream &os, const Law &) const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const SList species; //!< species within laws
                const Table topIndx; //!< topIndex[auxIndex] = topIndex
                Group      *next;    //!< for list
                Group      *prev;    //!< for list
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Group);
                LawList     laws;
                virtual ConstInterface & surrogate() const noexcept;
            public:
                const size_t maxUUID; //!< maximum of UUID.soze
            };


        }

    }

}

#endif
