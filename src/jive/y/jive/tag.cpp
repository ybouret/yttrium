
#include "y/jive/tag.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Tag:: ~Tag() noexcept {}
        Tag::  Tag(const char *  msg) : Tag_( new String(msg) ) {}
        Tag::  Tag(const String &msg) : Tag_( new String(msg) ) {}
        Tag::  Tag(const char    msg) : Tag_( new String(msg) ) {}
        Tag::  Tag(String       *str) noexcept : Tag_(str) { assert( 1 == (**this).quantity() ); }
        Tag::  Tag(const Tag    &tag) noexcept : Tag_(tag) {}


    }
}
