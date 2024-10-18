
#include "y/lingo/caption.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Y_SHALLOW_IMPL(AsCaption);
        
        Caption:: Caption(const String &_) : Caption_( new String(_) )
        {
        }

        Caption:: Caption(const char * const _) : Caption_( new String(_) )
        {
        }

        Caption:: Caption(const char _) : Caption_( new String(_) )
        {
        }

        Caption:: ~Caption() noexcept
        {
        }

        Caption:: Caption(const Caption &_) noexcept : Caption_(_) {}



        Caption:: Caption(const String * const _) noexcept :
        Caption_(_)
        {}


        std::ostream & operator<<(std::ostream &os, const Caption &_)
        {
            return os << *_;
        }

        
    }
}
