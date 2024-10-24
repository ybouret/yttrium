
#include "y/lingo/pattern/basic/excl.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Y_Lingo_Pattern_CallSign(Excl)


        Excl:: ~Excl() noexcept {}

        Excl::  Excl(const uint8_t _) noexcept :
        Pattern(UUID),
        byte(_)
        {
            Y_Lingo_Pattern(Excl);
        }

        Excl:: Excl(const Excl &_) noexcept :
        Pattern(_),
        byte(_.byte)
        {
            Y_Lingo_Pattern(Excl);
        }

        Pattern * Excl:: clone() const { return new Excl( *this ); }

        size_t Excl:: serialize(OutputStream &fp) const
        {
            const size_t ans = emitUUID(fp)+1;
            fp.write(byte);
            return ans;
        }

        bool Excl:: takes(Token &token, Source &source) const
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

        void  Excl:: query(CharDB &firstChars) const
        {
            CharDB mine;
            mine.fill(); assert(256==mine.size());
            mine.clr(byte);
            assert(255==mine.size());
            firstChars += mine;
        }

        String Excl:: regularExpression() const
        {
            String ans = '[';
            ans += '^';
            ans += ByteToRegExp(byte);
            ans += ']';
            return ans;
        }

        bool Excl:: univocal() const noexcept
        {
            return false;
        }

        bool Excl:: strong() const noexcept
        {
            return true;
        }

        void Excl:: viz(OutputStream &fp) const
        {
            Node(fp,this) << '[';
            Label(fp,&byte,1);
            fp << ",shape=doublecircle";
            fp << ']';
            Endl(fp);
        }

    }
}
