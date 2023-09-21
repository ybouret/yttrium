
#include "y/jive/lexical/scanner.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {
            
            Callback:: ~Callback() noexcept
            {

            }

        }
    }

}


namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {
            Scanner:: ~Scanner() noexcept {}

            void Scanner:: append(const Deed &deed)
            {
                if(!plan.insert(deed))
                    throw Specific::Exception( name->c_str(), "multiple callback '%s'", deed->name->c_str());
            }
        }
    }

}

