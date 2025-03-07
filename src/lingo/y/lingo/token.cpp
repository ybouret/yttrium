
#include "y/lingo/token.hpp"
#include <cstring>

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

        bool operator==(const char *lhs, const Token &rhs) noexcept
        {
            if(0==lhs)
            {
                return 0 == rhs.size;
            }
            else
            {
                size_t      len = strlen(lhs); if(rhs.size!=len) return false;
                const Char *chr = rhs.head;
                while(len-- > 0 )
                {
                    if( *(++lhs) != **chr) return false;
                    chr=chr->next;
                }
                return true;
            }
        }

        bool operator==(const char   lhs, const Token &rhs) noexcept
        {
            return 1 == rhs.size && lhs == **rhs.head;
        }

    }

}

#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Apex::Natural Token:: toNatural()   const
        {
            assert(size>0);

            apn       res;
            const apn ten = 10;
            for(const Char *ch=tail;ch;ch=ch->prev)
            {
                assert( isdigit(**ch) );
                res *= 10;
                res += char(**ch)-'0';
            }
            
            return res;
        }

    }

}
