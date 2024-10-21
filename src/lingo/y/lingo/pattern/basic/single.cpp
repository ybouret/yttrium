#include "y/lingo/pattern/basic/single.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Single:: ~Single() noexcept {}
        
        Single::  Single(const uint8_t _) noexcept :
        Pattern(UUID),
        byte(_)
        {
        }

        Single:: Single(const Single &_) noexcept :
        Pattern(_),
        byte(_.byte)
        {
        }

        Pattern * Single:: clone() const { return new Single( *this ); }

        size_t Single:: serialize(OutputStream &fp) const
        {
            const size_t ans = emitUUID(fp)+1;
            fp.write(byte);
            return ans;
        }

        bool Single:: takes(Token &token, Source &source) const
        {
            assert( 0 == token.size );
            Char *ch = source.get();
            if(0!=ch)
            {
                if(byte == **ch)
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

        void  Single:: query( CharDB &firstChars ) const
        {
            firstChars.set(byte);
        }

        String Single:: regularExpression() const
        {
            return ByteToRegExp(byte);
        }

        bool Single:: univocal() const noexcept
        {
            return true;
        }

        bool Single:: strong() const noexcept
        {
            return true;
        }

    }
}
