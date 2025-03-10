//! \file


#ifndef Y_Chemical_Assembly_Included
#define Y_Chemical_Assembly_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/text/justify.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Assembly
        {
        public:
            explicit Assembly() noexcept;
            virtual ~Assembly() noexcept;

            void enroll(const Entity &);


            std::ostream &pad(std::ostream &os,
                              const Entity &entity) const
            {
                for(size_t i=entity.name->size();i<maxNameLength;++i) os << ' ';
                return os;
            }

#if 0
            std::ostream & print(std::ostream        &os,
                                 const String        &msg,
                                 const Entity        &entity,
                                 const Justify::Type &format) const
            {

            }
#endif

            const size_t maxNameLength;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Assembly);
        };
    }

}

#endif

