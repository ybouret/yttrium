
//! \file

#ifndef Y_Chemical_Assembly_Included
#define Y_Chemical_Assembly_Included 1

#include "y/chemical/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Assembly
        {
        public:
            explicit Assembly() noexcept;
            virtual ~Assembly() noexcept;

            void enroll(const Entity &) noexcept;

            template <typename OSTREAM> inline
            OSTREAM & lj(OSTREAM      &os,
                         const String &st) const
            {
                os << st;
                for(size_t i=st.size();i<maxLen;++i) os << ' ';
                return os;
            }

            const size_t maxLen;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Assembly);
        };
    }

}

#endif

