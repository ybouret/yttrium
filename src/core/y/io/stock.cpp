
#include "y/io/stock.hpp"

namespace Yttrium
{
    namespace IO
    {

        Stock::  Stock() noexcept : CxxPoolOf<Char>() {}
        Stock:: ~Stock() noexcept {}


        void Stock:: reserve(size_t n)
        {
            while(n-- > 0) store( new Char(0) );
        }
    }

}

