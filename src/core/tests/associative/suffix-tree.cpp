#include "y/associative/suffix/tree.hpp"
#include "y/io/libc/input.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(associative_suffix_tree)
{
    SuffixTree tree;
    int        data = 5;
    
    Y_CHECK(tree.insert("hello",5, &data));
    Y_CHECK(tree.insert("hello",1, &data));
    Y_CHECK( tree.insert("hello",2, &data) );

    if(argc>1)
    {
        Libc::InputFile fp(argv[1]);
        String line;
        while(fp.gets(line))
        {
            std::cerr << line << std::endl;
            tree.insert(line,&data);
        }
    }

}
Y_UDONE()


