

#include "y/lingo/pattern/basic/void.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Void:: ~Void() noexcept {}

        Void::  Void() noexcept :
        Pattern(UUID)
        {
            Y_Lingo_Pattern(Void);
        }

        Void:: Void(const Void &_) noexcept :
        Pattern(_)
        {
            Y_Lingo_Pattern(Void);
        }

        Pattern * Void:: clone() const { return new Void( *this ); }

        size_t Void:: serialize(OutputStream &fp) const
        {
            return emitUUID(fp);
        }

        bool Void:: takes(Token & , Source & ) const
        {
            return false;
        }

        void  Void:: query( CharDB &firstChars ) const
        {
            firstChars.free();
        }

        String Void:: regularExpression() const
        {
            return "[^\\x00-\\xff]";
        }

        bool Void:: univocal() const noexcept
        {
            return true;
        }

        bool Void:: strong() const noexcept
        {
            return true;
        }

        void Void:: viz(OutputStream &fp) const
        {
            Node(fp,this) << "[label=\"\",shape=star]";
            Endl(fp);
        }

    }
}
