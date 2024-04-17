
//! \file

#ifndef Y_Chemical_Entities_Included
#define Y_Chemical_Entities_Included 1

#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Entities
        {
        public:
            explicit Entities() noexcept;
            virtual ~Entities() noexcept;

            void updateWith(const Entity &entity) noexcept;

            template <typename OSTREAM> inline
            OSTREAM & pad(OSTREAM &os, const Entity &entity) const
            {
                for(size_t i=entity.name.size();i<maxLength;++i) os << ' ';
                return os;
            }


            const size_t maxLength;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entities);
        };

    }

}

#endif

