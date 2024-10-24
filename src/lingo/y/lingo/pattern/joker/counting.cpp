


#include "y/lingo/pattern/joker/counting.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Y_Lingo_Pattern_CallSign(Counting)


        Counting:: ~Counting() noexcept
        {
        }

        Counting:: Counting(Pattern *m, const size_t nmin, const size_t nmax) noexcept :
        Joker(UUID,m),
        minimalCount(nmin),
        maximalCount(nmax)
        {
            Y_Lingo_Pattern(Counting);
            if(nmax<nmin) CoerceSwap(nmin,nmax);
        }

        size_t Counting:: serialize(OutputStream &fp) const
        {
            size_t ans = emitUUID(fp);
            ans += fp.emitVBR(minimalCount);
            ans += fp.emitVBR(maximalCount);
            return ans + motif->serialize(fp);
        }

        Pattern * Counting:: Create(Pattern *m, const size_t nmin, const size_t nmax)
        {
            assert(0!=m);
            try {
                if(m->feeble()) throw Specific::Exception("Lingo::Counting","pattern is not strong!");
                return new Counting(m,nmin,nmax);
            }
            catch(...)
            {
                delete m;
                throw;
            }
        }

        Pattern * Counting:: clone() const
        {
            assert(0!=motif);
            return Create(motif->clone(), minimalCount, maximalCount);
        }

        String Counting:: regularExpression() const
        {
            String ans = "(" + motif->regularExpression() + ")";

            ans += Formatted::Get("[%u,%u]", unsigned(minimalCount), unsigned(maximalCount));

            return ans;
        }

        bool Counting:: univocal() const noexcept
        {
            return minimalCount == maximalCount && motif->univocal();
        }

        bool Counting:: strong() const noexcept
        {
            assert(0!=motif);
            assert(motif->strong());
            return minimalCount > 0;
        }

        bool Counting:: takes(Token &token, Source &source) const
        {
            assert(0==token.size);
            Token  local;

            for(size_t i=0;i<=minimalCount;++i)
            {
                Token temp;
                if(motif->takes(temp,source))
                {
                    local << temp;
                }
                else
                {
                    assert(0==temp.size);
                    source.put(local);
                    return false;
                }
            }

            for(size_t i=minimalCount;i<=maximalCount;++i)
            {
                Token temp;
                if(motif->takes(temp,source))
                {
                    local << temp;
                    continue;
                }
                else
                {
                    break;
                }
            }

            token.swapWith(local);
            return true;
        }


        void Counting:: viz(OutputStream &fp) const
        {
            motif->viz(fp);
            Node(fp,this) << '[';
            const String text = minimalCount == maximalCount ? Formatted::Get("{%u}", unsigned(minimalCount) ) : Formatted::Get("{%u,%u}", unsigned(minimalCount), unsigned(maximalCount) );
            Label(fp,text);
            fp << ",shape=component";
            fp << ']';
            Endl(fp);

            Endl( Arrow(fp,this,motif) );

        }

    }

}

