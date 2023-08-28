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
#include "nw13.hxx"
#include "nw14.hxx"
#include "nw15.hxx"
#include "nw16.hxx"
#include "nw17.hxx"
#include "nw18.hxx"
#include "nw19.hxx"
#include "nw20.hxx"
#include "nw21.hxx"
#include "nw22.hxx"
#include "nw23.hxx"
#include "nw24.hxx"
#include "nw25.hxx"
#include "nw26.hxx"
#include "nw27.hxx"
#include "nw28.hxx"
#include "nw29.hxx"
#include "nw30.hxx"
#include "nw31.hxx"
#include "nw32.hxx"

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
            Y_NWT(12),
            Y_NWT(13),
            Y_NWT(14),
            Y_NWT(15),
            Y_NWT(16),
            Y_NWT(17),
            Y_NWT(18),
            Y_NWT(19),
            Y_NWT(20),
            Y_NWT(21),
            Y_NWT(22),
            Y_NWT(23),
            Y_NWT(24),
            Y_NWT(25),
            Y_NWT(26),
            Y_NWT(27),
            Y_NWT(28),
            Y_NWT(29),
            Y_NWT(30),
            Y_NWT(31),
            Y_NWT(32)
        };

        const unsigned      Tables:: MaxSize = sizeof(Tables_)/sizeof(Tables_[0])+1;
        const Table * const Tables:: CxxData = Tables_ - 2;
    }

}


