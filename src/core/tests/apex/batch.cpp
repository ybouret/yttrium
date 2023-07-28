#include "y/apex/m/batch.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/random/fill.hpp"
#include "y/type/complex.hpp"

using namespace Yttrium;


namespace
{

    struct Dummy
    {
        char ch[20];
    };

    template <typename T>
    static inline void checkBatch(Random::Bits &ran)
    {
        std::cerr << "-- testing Batch, sizeof=" << sizeof(T) << std::endl;

        for(size_t iter=0;iter<4;++iter)
        {
            const size_t   numObjects = ran.leq(1000);
            Apex::Batch<T> batch( numObjects );

            std::cerr << "  numObjects  = " << numObjects << std::endl;
            std::cerr << "  batch.count = " << batch.count << std::endl;
            std::cerr << "  batch.shift = " << batch.shift << std::endl;
            std::cerr << "  batch.dshft = " << batch.dataShift << std::endl;
            std::cerr << "  "; Y_CHECK( size_t(1) << batch.dataShift >= batch.count * sizeof(T) );

        }
    }

}

Y_UTEST(apex_batch)
{
    Random::Rand ran;


    checkBatch<float>(ran);
    checkBatch<Dummy>(ran);
    checkBatch< Complex<long double> >(ran);

}
Y_UDONE()

