
#include "y/chemical/weasel.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Weasel:: update(Library &lib, Equilibria &eqs, XNode *const root)
        {
            assert(0!=root);
            assert(root->defines<Weasel>());

            XList &list = root->branch();
            XList  temp;

            while(list.size>0)
            {
                {
                    AutoPtr<XNode> node = list.popHead();


                    if(node->defines<Formula>())
                    {
                        // append/check new species in library
                        const Formula formula( node.yield() );
                        const String  uuid = formula.uuid();
                        (void) lib(uuid);
                        continue;
                    }

                    if(node->defines<Equilibrium>())
                    {
                        // compile and append equilibrium to eqs
                        const Equilibrium::Pointer eq( compile(lib,& *node,eqs.nextIndex()) );
                        eqs(eq);
                        continue;
                    }

                }

                temp.pushTail( list.popHead() );
            }

            list.swapWith(temp);

        }

        void Weasel:: operator()(Library    &   lib,
                                 Equilibria &    eqs,
                                 Lingo::Module * const inputModule)
        {
            AutoPtr<XNode> root = parse(inputModule);
            update(lib,eqs,& *root);
        }

    }

}


