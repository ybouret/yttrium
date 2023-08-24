#ifndef Y_Jive_Tag_Included
#define Y_Jive_Tag_Included 1


#include "y/string.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Jive
    {

        typedef ArkPtr<String,const String> Tag_;

        class Tag : public Tag_
        {
        public:
            explicit Tag(const char *);
            explicit Tag(const String &);
            explicit Tag(const char);
            explicit Tag(String *   ) noexcept;
            explicit Tag(const Tag &) noexcept;
            virtual ~Tag() noexcept;

        private:
            Y_DISABLE_ASSIGN(Tag);
        };

    }
}

#endif

