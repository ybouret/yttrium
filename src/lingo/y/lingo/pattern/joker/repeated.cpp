

#include "y/lingo/pattern/joker/repeated.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Repeated:: ~Repeated() noexcept
        {
        }

        Repeated:: Repeated(Pattern *m, const size_t nmin) noexcept :
        Joker(UUID,m),
        minimalCount(nmin)
        {
        }

        size_t Repeated:: serialize(OutputStream &fp) const
        {
            size_t ans = emitUUID(fp);
            ans += fp.emitVBR(minimalCount);
            return ans + motif->serialize(fp);
        }

        Pattern * Repeated:: Create(Pattern *m, const size_t nmin)
        {
            assert(0!=m);
            try {
                if(m->feeble()) throw Specific::Exception("Lingo::Repeated","pattern is not strong!");
                return new Repeated(m,nmin);
            }
            catch(...)
            {
                delete m;
                throw;
            }
        }

        Pattern * Repeated:: clone() const
        {
            assert(0!=motif);
            return Create( motif->clone(), minimalCount );
        }

        String Repeated:: regularExpression() const
        {
            String ans = "(" + motif->regularExpression() + ")";
            switch(minimalCount)
            {
                case 0: ans += '*'; break;
                case 1: ans += '+'; break;
                default:
                    ans += Formatted::Get("[%u,]", unsigned(minimalCount));
            }
            return ans;
        }

        bool Repeated:: univocal() const noexcept
        {
            return false;
        }

        bool Repeated:: strong() const noexcept
        {
            return false;
        }

        bool Repeated:: takes(Token &token, Source &source) const
        {
            assert(0==token.size);
            Token  local;
            size_t count = 0;
            {
                Token temp;
                while(motif->takes(temp,source))
                {
                    ++count;
                    local << temp;
                }
            }

            if(count>=minimalCount)
            {
                token.swapWith(local);
                return true;
            }
            else
            {
                source.put(local);
                return false;
            }
        }

    }

}
