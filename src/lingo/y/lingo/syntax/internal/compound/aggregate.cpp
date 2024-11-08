
#include "y/lingo/syntax/internal/compound/aggregate.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Aggregate:: ~Aggregate() noexcept
            {
            }
            
            Aggregate & Aggregate:: operator += (const Rule &rule)
            {
                add(rule);
                return *this;
            }

            void Aggregate:: viz(OutputStream &fp) const
            {
                Node(fp,this) << "[";
                Label(fp, *name);
                fp << ",shape=house";
                Endl(fp << "]");
                vizLink(fp);
            }

            bool Aggregate:: robust() const  
            {
                checkNotEmpty("Aggregate::robust()",name->c_str());

                throw Exception("todo");
            }

            bool Aggregate:: accepts(Y_Lingo_Syntax_Args) const
            {
                assert(manifest.size>0);

                //--------------------------------------------------------------
                //
                //
                // prepare local tree
                //
                //
                //--------------------------------------------------------------
                XNode *        localTree = XNode::CreateFrom(*this);
                AutoPtr<XNode> guardTree = localTree;

                for(const RNode *node=manifest.head;node;node=node->next)
                {
                    const Rule &rule = **node;
                    if(rule.accepts(lexer,source,localTree)) continue;

                    //----------------------------------------------------------
                    //
                    // missed...
                    //
                    //----------------------------------------------------------
                    XNode::BackToLexer(lexer, guardTree.yield());
                    return false;
                }

                //--------------------------------------------------------------
                //
                //
                // all are accepted : grow tree with localTree
                //
                //
                //--------------------------------------------------------------
                assert(localTree->isWellFormed());
                XNode::Grow(tree,guardTree.yield());
                assert(tree->isWellFormed());
                return true;
            }
        }

    }

}
