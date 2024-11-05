#include "y/lingo/pattern/basic/byte.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Y_Lingo_Pattern_CallSign(Byte)


        Byte:: ~Byte() noexcept {}

        Byte::  Byte(const uint8_t _) noexcept :
        Pattern(UUID),
        byte(_)
        {
            Y_Lingo_Pattern(Byte);
        }

        Byte:: Byte(const Byte &_) noexcept :
        Pattern(_),
        byte(_.byte)
        {
            Y_Lingo_Pattern(Byte);
        }

        Pattern * Byte:: clone() const { return new Byte( *this ); }

        size_t    Byte:: serialize(OutputStream &fp) const
        {
            const size_t ans = emitUUID(fp)+1;
            fp.write(byte);
            return ans;
        }

        bool Byte:: takes(Y_Lingo_Pattern_Args) const
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
                // End of Source
                return false;
            }

        }

        void  Byte:: query( CharDB &firstChars ) const
        {
            firstChars.set(byte);
        }

        String Byte:: regularExpression() const
        {
            return ByteToRegExp(byte);
        }

        bool Byte:: univocal() const noexcept
        {
            return true;
        }

        bool Byte:: strong() const noexcept
        {
            return true;
        }

        void Byte:: viz(OutputStream &fp) const
        {
            Node(fp,this) << '[';
            Label(fp,&byte,1);
            fp << ",shape=box";
            fp << ']';
            Endl(fp);
        }

    }
}
