
#include "y/lingo/lexer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Lexer:: ~Lexer() noexcept
        {
            
        }

        void Lexer:: initialize()
        {
            withhold();
            const ScanPtr self = this;
            if(!scanners.insert(self))
                throw Specific::Exception(name->c_str(),"unexpected initialize failure");
        }
    }

}


