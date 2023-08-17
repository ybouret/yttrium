
#include "y/text/ascii/embedding.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;


Y_UTEST(text_embedding)
{
    Vector<String> txt(256,AsCapacity);
    for(size_t i=0;i<256;++i)
    {
        txt << FormatString("\\\\x%02x",unsigned(i));
    }



    txt('\a') = "\\\\\\\\a";
    txt('\b') = "\\\\\\\\b";
    txt('\f') = "\\\\\\\\f";
    txt('\n') = "\\\\\\\\n";
    txt('\r') = "\\\\\\\\r";
    txt('\t') = "\\\\\\\\t";
    txt('\v') = "\\\\\\\\v";

    for(size_t i=32;i<=126;++i)
    {
        txt[i+1] = char(i);
    }

    txt('"')  = "\\\\\\\"";
    txt('\'') = "\\'";
    txt('\\') = "\\\\\\\\";

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

    {
        Libc::OutputFile fp("embedding.dot");
        fp << "digraph G {\n";
        fp << " rankdir=\"TB\";\n";
        for(unsigned i=1;i<=256;++i)
        {
            //fp("%u [label=\"%s\"];\n",i,txt[i]());
            fp("%u [label=\"%s\"];\n",i,ASCII::Embedding::Char[i-1]);
        }
        fp << "}\n";
    }

}
Y_UDONE()
