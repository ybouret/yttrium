
#include "y/lingo/pattern/dictionary.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Dictionary:: ~Dictionary() noexcept
        {
        }

        Dictionary:: Dictionary()
        {
        }
        
        void Dictionary:: MultipleEntry(const String &name)
        {
            throw Specific::Exception("Lingo::Dictionary", "mutliple '%s", name.c_str());
        }
    }

}
