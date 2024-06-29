

#include "y/text/base64/encode.hpp"
#include "y/text/base64/decode.hpp"

#include "y/utest/run.hpp"
#include "y/memory/out-of-reach.hpp"
#include <cstring>

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

    char    output[8] = { 0 };
    uint8_t code[8]   = { 0 };

    const char * const tables[2] = { Base64::Encode::Table, Base64::Encode::TableURL };
    for(size_t t=0;t<2;++t)
    {
        const char * const table = tables[t];
        


        Y_STATIC_ZARR(output);
        Base64::Encode::_1(output, 'Y', table, false); std::cerr << output << std::endl;
        Y_STATIC_ZARR(output);
        Base64::Encode::_1(output, 'Y', table, true); std::cerr << output << std::endl;

        Y_STATIC_ZARR(output);
        Base64::Encode::_2(output, 'Y', 'b', table, false); std::cerr << output << std::endl;
        Y_STATIC_ZARR(output);
        Base64::Encode::_2(output, 'Y', 'b', table, true); std::cerr << output << std::endl;

        Y_STATIC_ZARR(output);
        Base64::Encode::_3(output, 'Y', 'b', '1', table); std::cerr << output << std::endl;

        std::cerr << "Loop..." << std::endl;
        unsigned count = 0;
        for(unsigned i=0;i<256;++i)
        {
            Y_STATIC_ZARR(output);
            Y_STATIC_ZARR(code);
            Base64::Encode::_1(output, char(i), table, false);
            Base64::Decode::_1(code,output[0],output[1]); Y_ASSERT(i==code[0]);
            
            for(unsigned j=0;j<256;++j)
            {
                Base64::Encode::_2(output, char(i), char(j), table, false);
                Base64::Decode::_2(code,output[0],output[1],output[2]); 
                Y_ASSERT(i==code[0]);
                Y_ASSERT(j==code[1]);

                for(unsigned k=0;k<256;++k,++count)
                {
                    Base64::Encode::_3(output, char(i), char(j), char(k), table );
                    Base64::Decode::_3(code,output[0],output[1],output[2],output[3]);
                    Y_ASSERT(i==code[0]);
                    Y_ASSERT(j==code[1]);
                    Y_ASSERT(k==code[2]);
                }
            }
        }
        std::cerr << "count=" << count << std::endl;
    }

    std::cerr << "LengthFor(1)=" << Base64::Encode::LengthFor(1,false) << std::endl;
    std::cerr << "LengthFor(2)=" << Base64::Encode::LengthFor(2,false) << std::endl;
    std::cerr << "LengthFor(4)=" << Base64::Encode::LengthFor(4,false) << std::endl;
    std::cerr << "LengthFor(8)=" << Base64::Encode::LengthFor(8,false) << std::endl;

    std::cerr << "With Padding: " << std::endl;
    std::cerr << "LengthFor(1)=" << Base64::Encode::LengthFor(1,true) << std::endl;
    std::cerr << "LengthFor(2)=" << Base64::Encode::LengthFor(2,true) << std::endl;
    std::cerr << "LengthFor(4)=" << Base64::Encode::LengthFor(4,true) << std::endl;
    std::cerr << "LengthFor(8)=" << Base64::Encode::LengthFor(8,true) << std::endl;

    int8_t decode[256];
    memset(decode,-1,sizeof(decode));
    for(size_t t=0;t<2;++t)
    {
        const char * const table = tables[t];
        for(size_t i=0;i<64;++i)
        {
            const unsigned c = table[i];
            decode[c] = i;
        }
    }

    {
        size_t k = 0;
        for(size_t i=0;i<16;++i)
        {
            for(size_t j=0;j<16;++j,++k)
            {
                std::cerr << std::setw(3) << int(decode[k]);
                if(k<255) std::cerr << ',';
            }
            std::cerr << std::endl;
        }

    }


}
Y_UDONE()
