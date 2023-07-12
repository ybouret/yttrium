
#include "y/random/shuffle.hpp"
#include "y/data/list/cxx.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

namespace {

    class Node
    {
    public:
        Node *next, *prev;
        size_t   data;

        Node(size_t a) noexcept : next(0), prev(0), data(a) {}
        ~Node() noexcept {}

        size_t operator*() const noexcept { return data; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Node);
    };
}

Y_UTEST(random_shuffle)
{
    Random::Rand ran;

    int arr[] = { 1, 2, 3, 4, 5};
    
    Core::Display(std::cerr << "arr=", arr, Y_STATIC_SIZE(arr)) << std::endl;
    Random::Shuffle::Range(arr,Y_STATIC_SIZE(arr),ran);
    Core::Display(std::cerr << "arr=", arr, Y_STATIC_SIZE(arr)) << std::endl;

    CxxListOf<Node> L;
    for(size_t i=1;i<=8;++i)
    {
        L.pushTail( new Node(i) );
    }
    std::cerr << "L=" << L << std::endl;
    Random::Shuffle::List(L,ran);
    std::cerr << "L=" << L << std::endl;
    
}
Y_UDONE()

