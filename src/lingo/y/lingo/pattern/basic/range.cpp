
#include "y/lingo/pattern/basic/range.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Range:: ~Range() noexcept {}

        Range::  Range(uint8_t lo, uint8_t up) noexcept :
        Pattern(UUID),
        lower(lo),
        upper(up)
        {
            if(upper<lower) CoerceSwap(lower,upper);
        }

        Range:: Range(const Range &_) noexcept :
        Pattern(_),
        lower(_.lower),
        upper(_.upper)
        {
        }

        Pattern * Range:: clone() const { return new Range( *this ); }

        size_t Range:: serialize(OutputStream &fp) const
        {
            const size_t ans = emitUUID(fp)+2;
            fp.write(lower);
            fp.write(upper);
            return ans;
        }

        bool Range:: takes(Token &token, Source &source) const
        {
            assert( 0 == token.size );
            Char *ch = source.get();
            if(0!=ch)
            {
                const uint8_t data = **ch;
                if(lower<=data && upper <=data)
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
                source.put(ch);
                return false;
            }

        }

        void  Range:: query( CharDB &firstChars ) const
        {
            const unsigned top = upper;
            for(unsigned i=lower;i<=top;++i)
                firstChars.set(i);
        }

        String Range:: regularExpression() const
        {
            String ans = '[';
            ans += ByteToRegExp(lower);
            ans += '-';
            ans += ByteToRegExp(upper);
            ans += ']';
            return ans;
        }


    }
}
