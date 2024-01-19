#include "y/sort/heap.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;


template <typename T>
static inline int compare(const T &lhs, const T &rhs)
{
    return lhs<rhs ? -1 : (rhs<lhs ? 1 : 0);
}

Y_UTEST(sort_heap)
{
    Random::Rand ran;

    
    double arr[100] = {0};
    for(size_t n=1;n<=100;++n)
    {
        for(size_t loop=0;loop<64;++loop)
        {
            for(size_t i=0;i<n;++i) arr[i] = double(i);
            Random::Shuffle::Range(arr,n,ran);

            Core::Display(std::cerr,arr,n) << std::endl;
            HeapSort::Tableau(arr,n,compare<double>);
            Core::Display(std::cerr,arr,n) << std::endl;
            for(size_t i=0;i<n;++i)
            {
                Y_ASSERT( size_t(arr[i]) == i );
            }
        }
    }


    {
        Vector<char>    ch;
        Vector<char>    cp;
        Vector<size_t>  id;
        {
            const char s[] = "World";
            for(size_t i=0;i<sizeof(s)-1;++i)
            {
                ch << s[i];
                cp << s[i];
                id << i;
            }
        }
        std::cerr << "ch=" << ch << std::endl;
        std::cerr << "cp=" << cp << std::endl;
        std::cerr << "id=" << id << std::endl;
        Y_CHECK(cp==ch);

        HeapSort::Call(ch,compare<char>,id);
        HeapSort::Call(cp,compare<char>);

        std::cerr << "ch=" << ch << std::endl;
        std::cerr << "cp=" << cp << std::endl;
        std::cerr << "id=" << id << std::endl;
        Y_CHECK(ch==cp);
    }

    {
        std::cerr << "Single Tab" << std::endl;
        double xx[5] = { 5, 4, 3, 2, 1};
        Core::Display(std::cerr,xx,sizeof(xx)/sizeof(xx[0])) << std::endl;
        HeapSort::Tableau(xx, sizeof(xx)/sizeof(xx[0]), compare<double>);
        Core::Display(std::cerr,xx,sizeof(xx)/sizeof(xx[0])) << std::endl;
    }

    {
        std::cerr << "Dual Tab" << std::endl;
        double xx[5] = { 5, 4, 3, 2, 1};
        short  id[5] = { 1, 2, 3, 4, 5};
        Core::Display(std::cerr,xx,sizeof(xx)/sizeof(xx[0])) << std::endl;
        Core::Display(std::cerr,id,sizeof(id)/sizeof(id[0])) << std::endl;

        HeapSort::Tableau(xx, sizeof(xx)/sizeof(xx[0]), compare<double>, id);
        Core::Display(std::cerr,xx,sizeof(xx)/sizeof(xx[0])) << std::endl;
        Core::Display(std::cerr,id,sizeof(id)/sizeof(id[0])) << std::endl;

    }







}
Y_UDONE()

