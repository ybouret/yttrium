
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
            Assembly(const Assembly &_) noexcept;

            void enroll(const Entity &) noexcept;

            template <typename OSTREAM> inline
            OSTREAM & print(OSTREAM            &os,
                            const char * const  pfx,
                            const Entity       &entity,
                            const char * const  sfx,
                            const Justify::Type jtype) const
            {
                const String  lhs = pfx;
                const String  rhs = sfx;
                const String  out = lhs + entity.key() + rhs;
                const Justify jst(out,maxKeySize+lhs.size()+rhs.size(),jtype);
                return os << jst;
            }

            const size_t maxKeySize;
        private:
            Y_DISABLE_ASSIGN(Assembly);
        };

    }

}

#endif
