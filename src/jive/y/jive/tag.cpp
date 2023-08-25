
#include "y/jive/tag.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Tag:: ~Tag() noexcept {}
        Tag::  Tag(const char *  msg) : Tag_( new String(msg) ) {}
        Tag::  Tag(const String &msg) : Tag_( new String(msg) ) {}
        Tag::  Tag(const char    msg) : Tag_( new String(msg) ) {}
        Tag::  Tag(const Tag    &tag) noexcept : Tag_(tag) {}

        Y_SHALLOW_IMPL(AsTag);

    }
}
