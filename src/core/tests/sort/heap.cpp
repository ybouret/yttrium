#include "y/sort/heap.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;

#if 0
template <typename T>
static inline int compare(const void *lhs, const void *rhs, void *) noexcept
{
    assert(0!=lhs);
    assert(0!=rhs);
    const T &l = *static_cast<const T *>(lhs);
    const T &r = *static_cast<const T *>(rhs);
    return l<r ? -1 : (r<l ? 1 : 0 );
}
#endif

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
            Random::Shuffle::Tableau(arr,n,ran);

            Core::Display(std::cerr,arr,n) << std::endl;
            HeapSort::Call(arr-1,n,compare<double>);
            Core::Display(std::cerr,arr,n) << std::endl;
            for(size_t i=0;i<n;++i)
            {
                Y_ASSERT(size_t(arr[i]) == i);
            }
        }
    }






}
Y_UDONE()

