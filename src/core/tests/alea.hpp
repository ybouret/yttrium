#include <cmath>
#include <ctime>

namespace
{
    inline double alea()
    {
        return (0.5 + rand()) / (1.0+RAND_MAX);
    }

    inline size_t alea_leq(const size_t n)
    {
        return static_cast<size_t>(floor(double(n) * alea() + 0.5));
    }

    template <typename T> inline
    void alea_shuffle(T a[], const size_t n)
    {
        for(size_t i=n-1;i>0;--i)
        {
            const size_t j = alea_leq(i);
            Yttrium::Swap(a[i], a[j]);
        }
    }

}

