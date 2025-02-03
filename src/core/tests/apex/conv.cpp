
#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;
using namespace Apex;


#define TRY_CAST(TYPE) do { \
/**/ std::cerr << "tryCast<" << #TYPE << ">" << std::endl;\
/**/ TYPE t = 0; \
/**/ if( num.tryCast<TYPE>(t) ) { std::cerr << "Success @" << t << std::endl; } else {  std::cerr << "Failure" << std::endl; } \
} while(false)

Y_UTEST(apex_conv)
{
    {
        Natural num = 6;
        Natural den = 7;
        if(argc>2)
        {
            num = argv[1];
            den = argv[2];
        }

        std::cerr << num << "/" << den;
        std::cerr << " = " << Natural::Ratio<float>(num,den);
        std::cerr << " = " << Natural::Ratio<double>(num,den);
        std::cerr << " = " << Natural::Ratio<long double>(num,den);
        std::cerr << std::endl;

        std::cerr << "Trying to convert " << num << std::endl;

        TRY_CAST(uint8_t);
        TRY_CAST(int8_t);

        TRY_CAST(uint16_t);
        TRY_CAST(int16_t);

        TRY_CAST(uint32_t);
        TRY_CAST(int32_t);

        TRY_CAST(uint64_t);
        TRY_CAST(int64_t);
    }

    {
        Integer num = -7;
        std::cerr << "Trying to convert " << num << std::endl;
        TRY_CAST(uint8_t);
        TRY_CAST(int8_t);
        TRY_CAST(uint16_t);
        TRY_CAST(int16_t);
    }

}
Y_UDONE()
