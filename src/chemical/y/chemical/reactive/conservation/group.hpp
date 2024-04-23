
//! \file

#ifndef Y_Chemical_Conservation_Group_Included
#define Y_Chemical_Conservation_Group_Included 1

#include "y/chemical/reactive/conservation/law.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            typedef Small::BareLightList<const Law> clList; //!< alias
            typedef clList::NodeType                clNode; //!< alias

            //__________________________________________________________________
            //
            //
            //
            //! Group of dependent conservation laws
            //
            //
            //__________________________________________________________________
            class Group : public Object, public clList
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
                
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Group *next; //!< for list
                Group *prev; //!< for list
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Group);
            };


        }

    }

}

#endif
