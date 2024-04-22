
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
            typedef Small::BareLightList<const Law> clList;
            typedef clList::NodeType                clNode;

            class Group : public Object, public clList
            {
            public:
                typedef CxxListOf<Group> List;
                
                explicit Group(const Law &first);
                virtual ~Group() noexcept;

                Group *next;
                Group *prev;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Group);
            };


        }

    }

}

#endif
