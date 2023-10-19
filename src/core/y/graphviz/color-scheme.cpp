
#include "y/graphviz/color-scheme.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{
    namespace GraphViz
    {
        const ColorScheme ColorScheme::Table[] =
        {
            { "accent3", 3 },
            { "accent4", 4 },
            { "accent5", 5 },
            { "accent6", 6 },
            { "accent7", 7 },
            { "accent8", 8 },

            { "dark23", 3 },
            { "dark24", 4 },
            { "dark25", 5 },
            { "dark26", 6 },
            { "dark27", 7 },
            { "dark28", 8 }
        };

        const size_t ColorScheme:: Count = sizeof(Table)/sizeof(Table[0]);

        const ColorScheme * ColorScheme:: Get(const char *id)
        {
            assert(0!=id);

            for(size_t i=0;i<Count;++i)
            {
                const ColorScheme *scheme = &Table[i];
                if( 0 == strcmp(scheme->name,id) ) return scheme;
            }


            throw Specific::Exception("GraphViz::ColorScheme::Get","no '%s",id);
        }

    }

}

#include "y/string.hpp"

namespace Yttrium
{
    namespace GraphViz
    {

        const ColorScheme * ColorScheme:: Get(const String &id)
        {
            return Get(id.c_str());
        }
    }

}
