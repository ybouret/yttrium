
#include "y/lingo/pattern/joker/optional.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Optional:: ~Optional() noexcept
        {
        }

        Optional:: Optional(Pattern *m) noexcept : Joker(UUID,m)
        {
            Y_Lingo_Pattern(Optional);
        }

        Pattern * Optional:: Create(Pattern *m)
        {
            assert(0!=m);
            try {
                return new Optional(m);
            }
            catch(...)
            {
                delete m;
                throw;
            }
        }


        size_t Optional:: serialize(OutputStream &fp) const
        {
            const size_t ans = emitUUID(fp);
            return ans + motif->serialize(fp);
        }

        Pattern * Optional:: clone() const
        {
            assert(0!=motif);
            return Create( motif->clone() );
        }

        String Optional:: regularExpression() const
        {
            return  motif->regularExpression() + '?';
        }

        bool Optional:: univocal() const noexcept
        {
            return motif->univocal();
        }

        bool Optional:: strong() const noexcept
        {
            return false;
        }

        bool Optional:: takes(Token &token, Source &source) const
        {
            assert(0==token.size);
            (void) motif->takes(token,source);
            return true;
        }

        void Optional:: viz(OutputStream &fp) const
        {
            motif->viz(fp);
            Node(fp,this) << '[';
            Label(fp,"?");
            fp << ",shape=triangle";
            fp << ']';
            Endl(fp);

            Endl( Arrow(fp,this,motif) );

        }

    }

}

