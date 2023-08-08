
#include "y/jive/token.hpp"
namespace Yttrium
{
    namespace Jive
    {
        Token:: ~Token() noexcept {}

        Token:: Token() noexcept : Char::List() {}

        Token:: Token(const Token &other) : Char::List(other) {}

        String Token:: toString(const size_t skip,const size_t trim) const
        {
            const size_t drop = skip+trim;
            if(drop>=size) return String();

            size_t       num = size-drop;
            const Char  *chr = ListOps::Next(head,skip);
            String       res(num,AsCapacity,false);

            while(num-- > 0)
            {
                assert(0!=chr);
                res << **chr;
                chr=chr->next;
            }

            return res;
        }

        std::ostream & operator<<(std::ostream &os, const Token &token)
        {
            const String str = token.toString();
            return os << str;
        }


    }

}
