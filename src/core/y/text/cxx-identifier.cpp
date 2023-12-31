
#include "y/text/cxx-identifier.hpp"

namespace Yttrium
{
    String CxxIdentifier:: From(const String &s)
    {
        String out(s.size(),AsCapacity,false);

        for(size_t i=1;i<=s.size();++i)
        {
            char C = s[i];
            if( isalnum(C) ) goto DONE;
            C = '_';
        DONE:
            out << C;
        }

        return out;

    }
}
