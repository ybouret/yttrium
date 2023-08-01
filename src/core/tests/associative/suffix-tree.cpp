#include "y/associative/suffix/tree.hpp"
#include "y/io/libc/input.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;

Y_UTEST(associative_suffix_tree)
{

    int        data = 5;
    Vector<void *> addr;

    {
        SuffixTree tree;
        addr << tree.insert("hello",5, &data);
        addr << tree.insert("hello",1, &data);
        addr << tree.insert("hello",2, &data);

        for(size_t i=1;i<=addr.size();++i)
        {
            std::cerr << "path = " << tree.pathOf(addr[i]) << std::endl;
        }
    }

    {
        SuffixTree tree;
        if(argc>1)
        {
            Libc::InputFile fp(argv[1]);
            String line;
            addr.free();

            while(fp.gets(line))
            {
                //std::cerr << line << std::endl;
                void *ptr = tree.insert(line,&data);
                if(!ptr) std::cerr << "Multiple '" << line << "'" << std::endl;
                else
                    addr << ptr;
            }
        }
        for(size_t i=1;i<=addr.size();++i)
        {
            std::cerr << "path = " << tree.pathOf(addr[i]) << std::endl;
        }
    }

}
Y_UDONE()


