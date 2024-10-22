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

            std::cerr << "<sub>" << std::endl;

            while(curr<last)
            {
                const char c = *(curr++);
                
                switch(c)
                {

                        //------------------------------------------------------
                        //
                        // grouping
                        //
                        //------------------------------------------------------
                    case '(':
                        ++depth;
                        motif->pushTail( subExpr() );
                        break;

                    case ')':
                        if(--depth<0) throw Specific::Exception(CallSign,"extraneous '%c' in '%s'",c,start);
                        goto END;

                        //------------------------------------------------------
                        //
                        // one byte joker
                        //
                        //------------------------------------------------------
                    case '*':
                    case '+':
                    case '?':
                    case '&':
                        if(motif->size<=0) throw Specific::Exception(CallSign,"missing expression before '%c' after '%s'",c,start);
                        Jokerize(*motif,c);
                        break;

                        //------------------------------------------------------
                        //
                        // new byte
                        //
                        //------------------------------------------------------
                    default:
                        std::cerr << "adding '" << c << "'" << std::endl;
                        motif->add(c);
                        break;
                }

            }

        END:
            std::cerr << "<sub/>" << std::endl;

            if( motif->size <= 0 )
                return new Void();


            return motif.yield();
        }

    }

}

