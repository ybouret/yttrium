
#include "y/chemical/weasel/initial/compiler.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Initial
        {

            Compiler:: Compiler()
            {

            }

            Compiler:: ~Compiler() noexcept
            {

            }

            Design * Compiler:: operator()(const XNode &root,
                                           Library     &lib)
            {
                assert(root.defines<Design>());

                AutoPtr<Design> design = new Design();
                {
                    const XList &list = root.branch(); assert(list.size>0);
                    const XNode *curr = list.head;     assert(curr->is(Design::Logo));
                    {
                        String   name = curr->lexeme().toString();
                        Coerce(*(design->name)).swapWith(name);
                    }
                    
                    for(curr=curr->next;curr;curr=curr->next)
                    {

                    }
                }

                return design.yield();
            }
        }

    }

}

