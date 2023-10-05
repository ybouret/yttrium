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

        Tag:: Tag(const String *str) noexcept : Tag_(str) {}

        Y_SHALLOW_IMPL(AsTag);

    }
}
