#include "y/associative/suffix/core/tree.hpp"
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
        Core::SuffixTree tree;
        addr << tree.insert("hello",5, &data);
        addr << tree.insert("hello",1, &data);
        addr << tree.insert("hello",2, &data);

        for(size_t i=1;i<=addr.size();++i)
        {
            std::cerr << "path = " << tree.pathOf(addr[i]) << std::endl;
        }
    }

    {
        Core::SuffixTree tree;
        if(argc>1)
        {
            std::cerr << "loading " << argv[1] << std::endl;
            Libc::InputFile fp(argv[1]);
            String line;
            addr.free();

            while(fp.gets(line))
            {
                void *ptr = tree.insert(line,&data);
                if(!ptr) std::cerr << "Multiple '" << line << "'" << std::endl;
                else
                    addr << ptr;
            }
        }

        std::cerr << "got " << addr.size() << " item" << std::endl;
        for(size_t i=1;i<=addr.size();++i)
        {
            const String path = tree.pathOf(addr[i]);
            std::cerr << "path = " << path << std::endl;
            Y_ASSERT(&data == tree.search(path) );
        }
        String toto = "toto";
    }

}
Y_UDONE()


