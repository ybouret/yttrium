
#include "y/lingo/pattern/basic/lump.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Lump:: ~Lump() noexcept {}

        Lump::  Lump(uint8_t lo, uint8_t up) noexcept :
        Pattern(UUID),
        lower(lo),
        upper(up)
        {
            Y_Lingo_Pattern(Lump);
            if(upper<lower) CoerceSwap(lower,upper);
        }

        Lump:: Lump(const Lump &_) noexcept :
        Pattern(_),
        lower(_.lower),
        upper(_.upper)
        {
            Y_Lingo_Pattern(Lump);
        }

        unsigned Lump:: code() const noexcept
        {
            const unsigned lo = lower;
            const unsigned up = upper;
            return (up<<8) | lo;
        }

        Pattern * Lump:: clone() const { return new Lump( *this ); }

        size_t Lump:: serialize(OutputStream &fp) const
        {
            const size_t ans = emitUUID(fp)+2;
            fp.write(lower);
            fp.write(upper);
            return ans;
        }

        bool Lump:: takes(Token &token, Source &source) const
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

        void  Lump:: query( CharDB &firstChars ) const
        {
            const unsigned top = upper;
            for(unsigned i=lower;i<=top;++i)
                firstChars.set(i);
        }

        String Lump:: regularExpression() const
        {
            String ans = '[';
            ans += ByteToRegExp(lower);
            ans += '-';
            ans += ByteToRegExp(upper);
            ans += ']';
            return ans;
        }

        bool Lump:: univocal() const noexcept
        {
            return lower>=upper;;
        }

        bool Lump:: strong() const noexcept
        {
            return true;
        }

        void Lump:: viz(OutputStream &fp) const
        {
            Node(fp,this) << "[label=\"";
            const char text[4] = { char(lower), '-', char(upper), 0 };
            Text(fp,text);
            fp << "\",shape=rectangle]";
            Endl(fp);
        }
    }
}
