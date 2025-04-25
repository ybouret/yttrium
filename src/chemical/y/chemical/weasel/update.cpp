
#include "y/chemical/weasel.hpp"
#include "y/chemical/plexus/initial/design.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Weasel:: update(Library    &lib,
                             Equilibria &eqs,
                             Repertory  &rep,
                             XNode *const root)
        {
            assert(0!=root);
            assert(root->defines<Weasel>());

            XList &list = root->branch();
            XList  temp;

            while(list.size>0)
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

                if(node->defines<Initial::Design>())
                {
                    // extract code for Axioms and store into repository
                    const XCode xcode = node.yield();
                    rep(xcode);
                    continue;
                }


                temp.pushTail( node.yield() );
            }

            list.swapWith(temp);

        }

        void Weasel:: operator()(Library    &    lib,
                                 Equilibria &    eqs,
                                 Repertory  &    rep,
                                 Lingo::Module * const inputModule)
        {
            AutoPtr<XNode> root = parse(inputModule);
            update(lib,eqs,rep,& *root);
        }

    }

}


