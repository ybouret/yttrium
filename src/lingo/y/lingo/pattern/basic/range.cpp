
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
            Y_Lingo_Pattern(Range);
            if(upper<lower) CoerceSwap(lower,upper);
        }

        Range:: Range(const Range &_) noexcept :
        Pattern(_),
        lower(_.lower),
        upper(_.upper)
        {
            Y_Lingo_Pattern(Range);
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

        bool Range:: univocal() const noexcept
        {
            return lower>=upper;;
        }

        bool Range:: strong() const noexcept
        {
            return true;
        }

        void Range:: viz(OutputStream &fp) const
        {
            Node(fp,this) << "[label=\"";
            const char text[4] = { char(lower), '-', char(upper), 0 };
            Text(fp,text);
            fp << "\",shape=rectangle]";
            Endl(fp);
        }
    }
}
