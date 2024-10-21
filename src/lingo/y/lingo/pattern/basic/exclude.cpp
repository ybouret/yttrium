
#include "y/lingo/pattern/basic/exclude.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Exclude:: ~Exclude() noexcept {}

        Exclude::  Exclude(const uint8_t _) noexcept :
        Pattern(UUID),
        byte(_)
        {
        }

        Exclude:: Exclude(const Exclude &_) noexcept :
        Pattern(_),
        byte(_.byte)
        {
        }

        Pattern * Exclude:: clone() const { return new Exclude( *this ); }

        size_t Exclude:: serialize(OutputStream &fp) const
        {
            const size_t ans = emitUUID(fp)+1;
            fp.write(byte);
            return ans;
        }

        bool Exclude:: takes(Token &token, Source &source) const
        {
            assert( 0 == token.size );
            Char *ch = source.get();
            if(0!=ch)
            {
                if(byte != **ch)
                {
                    token.pushTail(ch);
                    return true;
                }
                else
                {
                    source.put(ch);
                    return false;
                }
            }
            else
            {
                return false;
            }

        }

        void  Exclude:: query(CharDB &firstChars) const
        {
            CharDB mine;
            mine.fill(); assert(256==mine.size());
            mine.clr(byte);
            std::cerr << "mine.size=" << mine.size() << std::endl;
            assert(255==mine.size());
            firstChars += mine;
        }

        String Exclude:: regularExpression() const
        {
            String ans = '[';
            ans += '^';
            ans += ByteToRegExp(byte);
            ans += ']';
            return ans;
        }

        bool Exclude:: univocal() const noexcept
        {
            return false;
        }

        bool Exclude:: strong() const noexcept
        {
            return true;
        }

    }
}
