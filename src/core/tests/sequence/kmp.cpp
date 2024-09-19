
#include "y/container/cxx/array.hpp"
#include "y/utest/run.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Algo
    {
        struct KMP
        {
            typedef Memory::Wad<int,Memory::Pooled> ArrayType;

            class Plan : ArrayType
            {
            public:
                template <typename T> inline
                explicit Plan(const T * const x, const int m) :
                ArrayType(m+1),
                next( lead() ),
                size( m )
                {
                    assert(0!=x);
                    assert(m>0);
                    assert(capacity>=size);
                    assert(numBytes>=size*sizeof(int));

                    int i = 0;
                    int j = next[0] = -1;
                    while (i < m) {
                        while (j > -1 && x[i] != x[j])
                        {
                            assert(j<=m);
                            j = next[j];
                        }
                        i++;
                        j++;
                        assert(i<=m);
                        assert(j<=m);
                        if (x[i] == x[j])
                            next[i] = next[j];
                        else
                            next[i] = j;
                    }
                }

                virtual ~Plan() noexcept {}

                Y_OSTREAM_PROTO(Plan);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plan);
                int * const  next;
                const size_t size;
            };

        };

        std::ostream & operator<<(std::ostream &os, const KMP::Plan &plan)
        {
            return Core::Display(os, plan.next, plan.size+1);
        }


    }
}

using namespace Yttrium;

Y_UTEST(sequence_kmp)
{
    const char x[] = "GCAGAGAG";
    const size_t m = strlen(x);
    std::cerr << "x=" << x << ", m=" << m << std::endl;
    Algo::KMP::Plan plan(x,m);
    std::cerr << "plan=" << plan << std::endl;
}
Y_UDONE()

