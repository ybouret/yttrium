
//! \file
#ifndef Y_Jive_Group_Included
#define Y_Jive_Group_Included 1


#include "y/jive/entity.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class Group
        {
        public:
            explicit Group() noexcept;
            virtual ~Group() noexcept;
            void     updateWith(const Entity &) noexcept;

            const size_t maxNameSize;

            template <typename OSTREAM> inline
            OSTREAM & pad(OSTREAM &os, const Entity &ent) const
            {
                const size_t n=ent.name->size();
                for(size_t i=n;i<maxNameSize;++i) os << ' ';
                return os;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Group);
        };

    }

}

#endif
