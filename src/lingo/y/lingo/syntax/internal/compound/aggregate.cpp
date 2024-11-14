
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
                *this << rule;
                return *this;
            }

            void Aggregate:: vizMark(OutputStream &fp) const
            {
                fp << ",shape=house";
                fp << ",style=\"";
                switch(type)
                {
                    case Definite: fp << "filled"; break;
                    case Grouping: fp << "dashed"; break;
                    case NoSingle: fp << "filled,rounded"; break;
                }
                fp << "\"";
            }

            
            bool Aggregate:: accepts(Y_Lingo_Syntax_Args) const
            {
                assert(manifest.size>0);
                Y_Lingo_Syntax_Rule_Emit(depth, "[Aggregate '" << name << "']");
                
                //--------------------------------------------------------------
                //
                //
                // prepare local tree
                //
                //
                //--------------------------------------------------------------
                XNode *        localTree = XNode::CreateFrom(*this);
                AutoPtr<XNode> guardTree = localTree;
                const size_t   ruleDepth = depth+1;
                
                for(const RNode *node=manifest.head;node;node=node->next)
                {
                    const Rule &rule = **node;
                    if(rule.accepts(lexer,source,localTree,ruleDepth)) continue;

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
                return true;

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


