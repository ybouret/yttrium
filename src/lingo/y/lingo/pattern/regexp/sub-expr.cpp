#include "y/lingo/pattern/regexp/compiler.hpp"
#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Pattern * RXC:: subExpr()
        {
            AutoPtr<Logic>     motif = new And();
            const char * const start = curr;

            while(curr<last)
            {
                const char c = *(curr++);
                
                switch(c)
                {

                    default:
                        std::cerr << "adding '" << c << "'" << std::endl;
                        motif->add(c);
                        break;
                }

            }

            std::cerr << "done" << std::endl;

            if( motif->size <= 0 ) throw Specific::Exception(CallSign, "empty sub expression after '%s'", start);


            return motif.yield();
        }

    }

}

