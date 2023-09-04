#include "y/associative/suffix/core/tree.hpp"
#include "y/stream/libc/input.hpp"
#include "y/stream/libc/output.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/graphviz/vizible.hpp"


using namespace Yttrium;

Y_UTEST(associative_suffix_tree)
{

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

            Vizible::GraphViz("tree.dot",tree);
        }

        addr.free();

        {
            Core::SuffixTree tree;
            if(argc>1)
            {
                std::cerr << "Loading " << argv[1] << std::endl;
                Libc::InputFile fp(argv[1]);
                String line;

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

            Vizible::GraphViz("input.dot",tree);

        }
    }

    {
        int data = 7;
        Core::SuffixTree tree;
        const String     key1 = "chemical";
        const String     key2 = "chemistry";
        const String     key3 = "chemically";
        void  *node1 = tree.insert(key1,&data);
        void  *node2 = tree.insert(key2,&data);
        void  *node3 = tree.insert(key3,&data);

        Vizible::GraphViz("full.dot",tree);
        tree.loosen(node3);
        Vizible::GraphViz("cut1.dot",tree);
        tree.loosen(node2);
        Vizible::GraphViz("cut2.dot",tree);
        tree.loosen(node1);
        

    }

}
Y_UDONE()


