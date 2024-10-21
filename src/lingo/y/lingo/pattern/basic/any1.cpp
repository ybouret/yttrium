
#include "y/lingo/pattern/basic/any1.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Any1:: ~Any1() noexcept {}

        Any1::  Any1() noexcept :
        Pattern(UUID)
        {
            Y_Lingo_Pattern(Any1);
        }

        Any1:: Any1(const Any1 &_) noexcept :
        Pattern(_)
        {
            Y_Lingo_Pattern(Any1);
        }

        Pattern * Any1:: clone() const { return new Any1( *this ); }

        size_t Any1:: serialize(OutputStream &fp) const
        {
            return emitUUID(fp);
        }

        bool Any1:: takes(Token &token, Source &source) const
        {
            assert( 0 == token.size );
            Char *ch = source.get();
            if(0!=ch)
            {
                token.pushTail(ch);
                return true;
            }
            else
            {
                return false;
            }

        }

        void  Any1:: query( CharDB &firstChars ) const
        {
            firstChars.fill();
        }

        String Any1:: regularExpression() const
        {
            return "[\\x00-\\xff]";
        }

        bool Any1:: univocal() const noexcept
        {
            return false;
        }

        bool Any1:: strong() const noexcept
        {
            return true;
        }

    }
}
