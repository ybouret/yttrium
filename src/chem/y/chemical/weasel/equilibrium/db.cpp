
#include "y/chemical/weasel/equilibrium/db.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        const char * EDB:: Table[] =
        {
#include   "db.hxx"
            NULL
        };

        const size_t EDB:: Count = sizeof(EDB::Table) / sizeof(EDB::Table[0]) - 1;
    }

}
