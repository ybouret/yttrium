#include "y/associative/address-book.hpp"
#include "y/utest/run.hpp"
#include "y/graphviz/vizible.hpp"
#include "y/random/mt19937.hpp"

using namespace Yttrium;


static inline SignType CompareBook(const void * const lhs, const void * const rhs) noexcept
{
    const int L = *(const int *)lhs;
    const int R = *(const int *)rhs;
    return Sign::Of(L,R);
}



Y_UTEST(associative_addrbook)
{
    Random::MT19937 ran;
    AddressBook     book;
    int a=1, b=2, c=3, d=4;

    Y_CHECK(book.insert(a));
    Y_CHECK(book.insert(b));
    Y_CHECK(book.insert(c));
    Y_CHECK(book.insert(d));

    GraphViz::Vizible::DotToPng("book.dot", book);

    for(AddressBook::Iterator it=book.begin();it!=book.end();++it)
    {
        const void *addr = *it;
        std::cerr << *(const int *)addr << std::endl;
    }

    book.display<int>(std::cerr << "book=" ) << std::endl;
    book.shuffle(ran);
    book.display<int>(std::cerr << "book=" ) << std::endl;

    book.sortByValue(CompareBook);
    book.display<int>(std::cerr << "book=" ) << std::endl;
    book.shuffle(ran);
    book.display<int>(std::cerr << "book=" ) << std::endl;

    book.sort( Type2Type<int>(), Comparison::Decreasing<int> );
    book.display<int>(std::cerr << "book=" ) << std::endl;



}
Y_UDONE()

