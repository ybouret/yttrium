
//! \file

#ifndef Y_Jive_Tags_Included
#define Y_Jive_Tags_Included 1

#include "y/string.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Jive
    {
        typedef ArkPtr<String,String> SharedString;

        class Tag : public SharedString
        {
        public:
            Tag(const Tag &)          noexcept;
            Tag(const SharedString &) noexcept;
            Tag(const String &);
            Tag(const char *, const size_t);
            Tag(const char *);
            Tag(const char c);
            virtual ~Tag() noexcept;
        private:
            Y_DISABLE_ASSIGN(Tag);
        };




    }
}

#endif

