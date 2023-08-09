#include "y/text/ascii/printable.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;


Y_UTEST(text_printable)
{
    Y_SIZEOF(ASCII::Printable::Char);

    Vector<String> txt(256,AsCapacity);
    for(size_t i=0;i<256;++i)
    {
        txt << FormatString("\\x%02x",unsigned(i));
    }

    

    txt['\a'+1] = "\\\\a";
    txt['\b'+1] = "\\\\b";
    txt['\f'+1] = "\\\\f";
    txt['\n'+1] = "\\\\n";
    txt['\r'+1] = "\\\\r";
    txt['\t'+1] = "\\\\t";
    txt['\v'+1] = "\\\\v";

    for(size_t i=32;i<=126;++i)
    {
        txt[i+1] = char(i);
    }

    txt[1+'"']  = "\\\"";
    txt[1+'\''] = "\\'";
    txt[1+'\\'] = "\\\\";

    size_t k = 0;
    for(size_t i=0;i<32;++i)
    {
        std::cerr << "\t";
        for(size_t j=0;j<8;++j)
        {
            std::cerr << '"' << txt[++k] << '"';
            if(k<256)
                std::cerr << ", ";
        }
        std::cerr << std::endl;
    }

}
Y_UDONE()
