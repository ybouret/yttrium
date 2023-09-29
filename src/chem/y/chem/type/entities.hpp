
//! \file


#ifndef Y_Chemical_Entitie_Included
#define Y_Chemical_Entities_Included 1

#include "y/chem/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! metrics to display entities
        //
        //
        //______________________________________________________________________
        class Entities
        {
        public:
            explicit Entities() noexcept;
            virtual ~Entities() noexcept;

            void updateWith(const Entity &) noexcept;

            template <typename OSTREAM> inline
            OSTREAM & pad(OSTREAM &os, const Entity &ent) const
            {
                for(size_t i=ent.name.size();i<maxNameSize;++i) os << ' ';
                return os;
            }

            const size_t maxNameSize;

        private:
            Y_DISABLE_ASSIGN(Entities);
        };

    }

}

#endif

