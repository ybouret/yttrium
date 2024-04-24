
//! \file

#ifndef Y_Chemical_Conservation_Group_Included
#define Y_Chemical_Conservation_Group_Included 1

#include "y/chemical/reactive/conservation/law.hpp"
#include "y/chemical/type/entities.hpp"


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
                typedef CxxListOf<Group> List; //!< alias
                
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

                void compile();                             //!< gather all species and make their AuxLevel indices

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const SList species; //!< species within laws
                Group      *next;    //!< for list
                Group      *prev;    //!< for list
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Group);
                LawList     laws;
                virtual ConstInterface & surrogate() const noexcept;
            };


        }

    }

}

#endif
