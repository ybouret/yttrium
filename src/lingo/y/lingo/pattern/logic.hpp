
//! \file

#ifndef Y_Lingo_Logic_Included
#define Y_Lingo_Logic_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        class Logic : public Pattern, public Pattern::List
        {
        protected:
            //! pattern must be protected
            explicit Logic(const uint32_t t) noexcept;
            Logic(const Logic &);

        public:
            virtual ~Logic() noexcept;

            virtual size_t serialize(OutputStream &fp) const;
            

        private:
            Y_DISABLE_ASSIGN(Logic);
        };

    }

}

#endif

