
#include "y/hashing/testing.hpp"
#include "y/hashing/function.hpp"
#include "y/hashing/md.hpp"
#include "y/memory/digest.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Hashing
    {

        void Testing:: Run(Function &H, const Testing tests[], const unsigned count)
        {
            assert(0!=tests);
            std::cerr << "<Hashing::Test " << H.name() << ">" <<std::endl;
            for(unsigned i=0;i<count;++i)
            {
                const Testing &the_test = tests[i];
                assert(0!=the_test.text);
                assert(0!=the_test.hash);
                const Digest hash = the_test.hash;
                const Digest hrun = MD::Of(H,the_test.text);
                std::cerr << "  " << hrun << '/' << hash << " <== '" << the_test.text << "'" << std::endl;
                if(hrun!=hash) throw Specific::Exception(H.name(),"failure test #%u",i+1);
            }
            std::cerr << "<Hashing::Test " << H.name() << "/>" <<std::endl;

        }

    }
}
