
#include "y/lingo/token.hpp"

namespace Yttrium
{
    namespace Lingo
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

        String Token:: toPrintable() const
        {
            String res;
            for(const Char *chr=head;chr;chr=chr->next)
            {
                res << chr->printable();
            }
            return res;
        }

        Token & Token:: operator<<(Token &token)
        {
            mergeTail(token);
            return *this;
        }

        Token & Token:: operator +=(const Token &token)
        {
            Token cpy(token);
            mergeTail(cpy);
            return *this;
        }

        Token & Token:: operator<<(Char * const ch) noexcept
        {
            assert(0!=ch);
            pushTail(ch);
            return *this;
        }

        Token & Token:: operator+=(const Char  &ch)
        {
            pushTail( new Char(ch) );
            return *this;
        }

        bool  operator==(const Token &lhs, const Token &rhs) noexcept
        {
            if(lhs.size!=rhs.size) return false;
            for(const Char *l=lhs.head,*r=rhs.head;l;l=l->next,r=r->next)
            {
                assert(0!=l);
                assert(0!=r);
                if( **l != **r ) return false;
            }
            return true;
        }

    }

}
