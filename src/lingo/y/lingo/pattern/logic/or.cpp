
#include "y/lingo/pattern/logic/or.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Or:: Or() noexcept :
        Logic(UUID)
        {
        }

        Or:: ~Or() noexcept {}

        Or:: Or(const Or &other) : Logic(other) {}

        Pattern * Or:: clone() const { return new Or(*this); }

        bool Or:: strong() const noexcept
        {
            return false;
        }

        bool Or:: univocal() const noexcept
        {
            return false;
        }

        void Or:: query(CharDB &fc) const
        {

        }

        String Or:: regularExpression() const
        {
            String ans;

            return ans;
        }

        bool Or:: takes(Token &token, Source &source) const
        {
            assert(0==token.size);

            return false;
        }



    }

}


