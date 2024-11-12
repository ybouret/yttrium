#include "y/lingo/syntax/grammar.hpp"
#include "y/system/exception.hpp"

namespace Yttrium {

    namespace Lingo
    {

        namespace Syntax
        {
            XNode * Grammar:: accept(Lexer &lexer, Source &source)
            {
                if(rules.size<=0) throw Specific::Exception(name->c_str(),"emtpy grammar");
                const Rule &primary = *(rules.head);
                XNode *     tree    = 0;
                if(primary.accepts(lexer,source,tree) )
                {
                    
                    return tree;
                }
                else
                {
                    assert(0==tree);

                    return tree;
                }

            }
        }
    }

}
