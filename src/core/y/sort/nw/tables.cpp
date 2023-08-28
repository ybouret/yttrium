#include "y/sort/nw/tables.hpp"

namespace Yttrium
{

    namespace NetworkSort
    {

#include "nw2.hxx"
#include "nw3.hxx"
#include "nw4.hxx"
#include "nw5.hxx"
#include "nw6.hxx"
#include "nw7.hxx"
#include "nw8.hxx"
#include "nw9.hxx"
#include "nw10.hxx"
#include "nw11.hxx"
#include "nw12.hxx"

#define Y_NWT(n) {nwTable##n,nwCount##n}

        const Table Tables_[] =
        {
            Y_NWT(2),
            Y_NWT(3),
            Y_NWT(4),
            Y_NWT(5),
            Y_NWT(6),
            Y_NWT(7),
            Y_NWT(8),
            Y_NWT(9),
            Y_NWT(10),
            Y_NWT(11),
            Y_NWT(12)
        };

        const unsigned      Tables:: MaxSize = sizeof(Tables_)/sizeof(Tables_[0])+1;
        const Table * const Tables:: CxxData = Tables_ - 2;
    }

}


