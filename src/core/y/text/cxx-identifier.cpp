
#include "y/text/cxx-identifier.hpp"
#include "y/container/algo/crop.hpp"
#include "y/container/algo/no-repeat.hpp"

namespace Yttrium
{
    namespace
    {
        static const char subst = '_';

        static inline bool isSubst(const char c) noexcept {
            return c == subst;
        }

        static inline
        void Compactify(String &s)
        {
            Algo::Crop(s,isSubst);
            Algo::NoRepeat(s,isSubst);
        }

    }

    String CxxIdentifier:: From(const String &s, const Style style)
    {
        String out(s.size(),AsCapacity,false);

        for(size_t i=1;i<=s.size();++i)
        {
            char C = s[i];
            if( isalnum(C) ) goto DONE;
            C = subst;
        DONE:
            out << C;
        }

        switch(style)
        {
            case Verbatim: break;
            case Compact: Compactify(out); break;
        }
        return out;

    }

    String CxxIdentifier:: From(const char *s, const Style style)
    {
        const String _(s);
        return From(_,style);
    }
}
