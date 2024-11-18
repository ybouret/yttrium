
#include "y/lingo/caption.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Y_SHALLOW_IMPL(AsCaption);
        
        Caption:: Caption(const String &_) :
        Caption_( new String(_) ), Memory::ReadOnlyBuffer()
        {
        }

        Caption:: Caption(const char * const _) :
        Caption_( new String(_) ), Memory::ReadOnlyBuffer()
        {
        }

        Caption:: Caption(const char _) :
        Caption_( new String(_) ), Memory::ReadOnlyBuffer()
        {
        }

        Caption:: ~Caption() noexcept
        {
        }

        Caption:: Caption(const Caption &_) noexcept :
        Caption_(_), Memory::ReadOnlyBuffer()
        {}



        Caption:: Caption(const String * const _) noexcept :
        Caption_(_), Memory::ReadOnlyBuffer()
        {}


        std::ostream & operator<<(std::ostream &os, const Caption &_)
        {
            return os << *_;
        }

        const void * Caption:: ro_addr() const noexcept
        {
            return (*this)->c_str();
        }

        size_t Caption:: measure() const noexcept
        {
            return (*this)->size();
        }
    }
}
