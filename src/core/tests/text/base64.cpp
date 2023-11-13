

#include "y/text/base64/encode.hpp"
#include "y/utest/run.hpp"
#include "y/memory/out-of-reach.hpp"

using namespace Yttrium;

namespace
{

}


Y_UTEST(text_base64)
{

    static const char table1[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    for(unsigned i=0,k=0;i<8;++i)
    {
        for(unsigned j=0;j<8;++j)
        {
            std::cerr << " '" << table1[k++] << "',";
        }
        std::cerr << std::endl;
    }

    std::cerr << std::endl;

    Base64::Encode::ShowInfo();

    char output[8] = { 0 };

    Y_STATIC_ZARR(output);
    Base64::Encode::_1(output, 'Y', false); std::cerr << output << std::endl;
    Y_STATIC_ZARR(output);
    Base64::Encode::_1(output, 'Y', true); std::cerr << output << std::endl;

    Y_STATIC_ZARR(output);
    Base64::Encode::_2(output, 'Y', 'b', false); std::cerr << output << std::endl;
    Y_STATIC_ZARR(output);
    Base64::Encode::_2(output, 'Y', 'b', true); std::cerr << output << std::endl;

    Y_STATIC_ZARR(output);
    Base64::Encode::_3(output, 'Y', 'b', '1'); std::cerr << output << std::endl;

    std::cerr << "Loop..." << std::endl;
    unsigned count = 0;
    for(unsigned i=0;i<256;++i)
    {
        Y_STATIC_ZARR(output);
        Base64::Encode::_1(output, char(i), false);
        for(unsigned j=0;j<256;++j)
        {
            Base64::Encode::_2(output, char(i), char(j), false);
            for(unsigned k=0;k<256;++k,++count)
            {
                Base64::Encode::_3(output, char(i), char(j), char(k) );
            }
        }
    }
    std::cerr << "count=" << count << std::endl;

}
Y_UDONE()
