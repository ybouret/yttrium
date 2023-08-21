

#include "y/text/justify.hpp"
#include "y/text/ops.hpp"
#include "y/string.hpp"

namespace Yttrium
{

    Justify:: ~Justify() noexcept {}

    Justify:: Justify(const Justify &other) noexcept :
    text(other.text),
    size(other.size),
    width(other.width),
    type(other.type)
    {
    }

    Justify:: Justify(const char * msg,
                      const size_t len,
                      const size_t w,
                      const Type   t) noexcept :
    text(msg),
    size(len),
    width(w),
    type(t)
    {
        assert(Good(text,size));
    }

    Justify:: Justify(const char * msg,
                      const size_t w,
                      const Type   t) noexcept :
    text(msg),
    size(StringLength(msg)),
    width(w),
    type(t)
    {
        assert(Good(text,size));
    }


    Justify:: Justify(const String &s,
                      const size_t  w,
                      const Type    t) noexcept :
    text(s.c_str()),
    size(s.size()),
    width(w),
    type(t)
    {
        assert(Good(text,size));
    }

    void Justify:: compute(size_t &prolog, size_t &epilog) const noexcept
    {
        prolog = epilog = 0;
        if(size<width)
        {
            const size_t space = width-size;
            switch(type)
            {
                case Left:  epilog = space; break;
                case Right: prolog = space; break;
                case Center: prolog = space/2; epilog = space-prolog; break;
            }
        }
    }
}


