

#include "y/memory/book.hpp"
#include "y/utest/run.hpp"
#include "../alea.hpp"
#include "y/lockable.hpp"

using namespace Yttrium;


Y_UTEST(memory_book)
{
    alea_seed();

    Y_SIZEOF(Memory::Book);

    Memory::Book book;


}
Y_UDONE()
