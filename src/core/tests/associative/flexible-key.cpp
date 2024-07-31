#include "y/associative/flexible-key.hpp"
#include "y/type/nullify.hpp"
#include "y/quantized.hpp"

#include "y/random/park-miller.hpp"
#include "y/random/fill.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;



#include "y/text/hexadecimal.hpp"

namespace {

    template <typename T> static inline
    void testFlex(Random::Bits &ran)
    {

        {
            FlexibleKey<T> key0(0,0);
            std::cerr << "key0=" << *key0 << std::endl;
            key0.displayHexadecimal(std::cerr << "key0=") << std::endl;
            FlexibleKey<T> cpy0(key0);
            key0.displayHexadecimal(std::cerr << "cpy0=") << std::endl;

        }
        for(size_t n=1; n <= 5; ++n)
        {
            CxxArray<T> data(n,0);
            std::cerr << "data = [";
            for(size_t i=1;i<=n;++i)
            {
                Random::Fill::Fuzzy( data[i], ran);
                std::cerr << ' ' << Hexadecimal(data[i]);
            }
            std::cerr << " ]" << std::endl;
            FlexibleKey<T> key0(&data[1],n);
            key0.displayHexadecimal(std::cerr << "key0=") << std::endl;
            FlexibleKey<T> cpy0(key0);
            key0.displayHexadecimal(std::cerr << "cpy0=") << std::endl;
        }
    }

}


Y_UTEST(associative_flexible_key)
{
    Random::ParkMiller ran;

    Y_SIZEOF( FlexibleKey<uint8_t> );
    Y_SIZEOF( FlexibleKey<uint64_t> );
    Y_SIZEOF( FlexibleKey<uint8_t>::Code );
    Y_SIZEOF( FlexibleKey<uint64_t>::Code );

    {
        const uint8_t data[] = { 1,2,3,4 };
        FlexibleKey<uint8_t> key(data,sizeof(data)/sizeof(data[1]));

        key.displayHexadecimal(std::cerr) << std::endl;
    }

    {
        const uint16_t data[] = { 0xabcd,2,3,4 };
        FlexibleKey<uint16_t> key(data,sizeof(data)/sizeof(data[1]));
        key.displayHexadecimal(std::cerr) << std::endl;
    }

    {
        const uint32_t data[] = { 0xabcd,2,3,4 };
        FlexibleKey<uint32_t> key(data,sizeof(data)/sizeof(data[1]));
        key.displayHexadecimal(std::cerr) << std::endl;
    }

    testFlex<uint8_t>(ran);
    testFlex<uint16_t>(ran);
    testFlex<uint32_t>(ran);
    testFlex<uint64_t>(ran);

}
Y_UDONE()

