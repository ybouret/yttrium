
#include "y/jive/lexical/scanner.hpp"

namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {
            Scanner:: ~Scanner() noexcept {}

            Action::Type Scanner::produce(const Token &tkn) const
            {
                std::cerr << "token: " << tkn << std::endl;
                return Action::Produce;
            }

        }
    }

}

