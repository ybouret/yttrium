#include "y/jive/tags.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Tag:: ~Tag() noexcept {}

        Tag:: Tag(const SharedString &s) noexcept : SharedString(s) {}

        Tag:: Tag(const String &      s) : SharedString( new String(s) ) {}

        Tag:: Tag(const char *msg, const size_t len) : SharedString( new String(msg,len) )
        {
        }

        Tag:: Tag(const char *msg) : SharedString( new String(msg) )
        {
        }

        Tag:: Tag(const char c) : SharedString( new String(c) )
        {

        }

        Tag:: Tag(const Tag &other) noexcept : SharedString( other ) {}
        





    }
}

