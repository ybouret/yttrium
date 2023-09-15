#include "y/associative/address-book.hpp"
#include "y/utest/run.hpp"
#include "y/graphviz/vizible.hpp"

using namespace Yttrium;


Y_UTEST(associative_addrbook)
{
    AddressBook book;
    int a=1, b=2;

    Y_CHECK(book.insert(a));
    Y_CHECK(book.insert(b));

    Vizible::GraphViz("book.dot", book);

    for(AddressBook::Iterator it=book.begin();it!=book.end();++it)
    {
        const void *addr = *it;
        std::cerr << *(const int *)addr << std::endl;
    }

}
Y_UDONE()

