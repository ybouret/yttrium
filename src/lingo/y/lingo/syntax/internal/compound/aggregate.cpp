
#include "y/lingo/syntax/internal/compound/aggregate.hpp"

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

#include "y/lingo/syntax/rule/visit.hpp"
#include "y/system/exception.hpp"
//#include "y/lingo/syntax/rules.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {


            bool Aggregate:: robust() const
            {
                static const char fn[] = "Aggregate::robust()";
                checkNotEmpty(fn,name->c_str());

                const Rule & self = *this;
                for(const RNode *node=manifest.head;node;node=node->next)
                {
                    const Rule & rule = **node;
                    {
                        const Visit  visited(rule);
                        if( visited.has(self) )
                            throw Specific::Exception(fn, "early recursive '%s' in '%s' during look up", self.name->c_str(), rule.name->c_str());
                    }
                    if( rule.robust() ) return true;
                }
                return false;
            }

        }
    }
}


