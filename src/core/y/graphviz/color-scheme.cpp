
#include "y/graphviz/color-scheme.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{
    namespace GraphViz
    {

#define Y_GV_CS_3_8(NAME) \
{ #NAME "3", 3 },         \
{ #NAME "4", 4 },         \
{ #NAME "5", 5 },         \
{ #NAME "6", 6 },         \
{ #NAME "7", 7 },         \
{ #NAME "8", 8 }

#define Y_GV_CS_3_9(NAME) \
Y_GV_CS_3_8(NAME),        \
{ #NAME "9", 9 }

        const ColorScheme ColorScheme::Table[] =
        {
            Y_GV_CS_3_8(accent),
            Y_GV_CS_3_8(dark2),
            Y_GV_CS_3_9(blues),
            Y_GV_CS_3_9(greens),
            Y_GV_CS_3_9(oranges),
            Y_GV_CS_3_9(pastel1),
            Y_GV_CS_3_8(pastel2),
            Y_GV_CS_3_9(set1),
            Y_GV_CS_3_8(set2)
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
