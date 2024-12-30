
#include "y/lingo/pattern/matching.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Matching:: ~Matching() noexcept
        {
        }

        Matching:: Matching(const Matching &_) :
        Token(),
        motif( _.motif->clone() )
        {
        }


        bool Matching:: exactly_(Module * const m)
        {
            assert(0!=m);
            Source source(m);
            release();
            if(motif->takes(*this,source,0))
            {
                return !source.ready();
            }
            else
            {
                return false;
            }
        }

        bool Matching:: somehow_(Module * const m)
        {
            assert(0!=m);
            Source source(m);
            release();

            while(!motif->takes(*this,source,0))
            {
                if(!source.ready()) return false;
                source.skip();
            }

            return true;
        }

        String Matching:: StringToRegExp(const String &str)
        {
            String rxp;
            const size_t n = str.size();
            for(size_t i=1;i<=n;++i)
            {
                const uint8_t byte( str[i] );
                const String  hexa = Formatted::Get("\\x%02x",byte);
                rxp << hexa;
            }
            return rxp;
        }

        String Matching:: StringToRegExp(const char * const str)
        {
            const String _(str);
            return StringToRegExp(_);
        }
    }
}
