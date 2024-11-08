

#include "y/lingo/syntax/internal/wildcard/repeat.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Repeat:: ~Repeat() noexcept
            {
            }


            Repeat:: Repeat(const Rule & _rule,
                            const size_t  nmin) :
            Wildcard(NameFor(_rule,nmin), UUID, _rule),
            atLeast(nmin)
            {
                Y_Lingo_Syntax_Rule(Repeat);
            }

            Caption Repeat:: NameFor(const Rule &r, const size_t n)
            {
                String s = *r.name;
                switch(n)
                {
                    case 0: s += '*'; break;
                    case 1: s += '+'; break;
                    default:
                        s += Formatted::Get(">=%u", unsigned(n));
                }
                return Caption(s);
            }


            bool Repeat:: robust() const { return atLeast>0; }

            void Repeat:: viz(OutputStream &fp) const
            {
                Node(fp,this) << "[";
                Label(fp,*name);
                fp << ",shape=component";
                Endl(fp << "]");
                Endl(Arrow(fp,this, &rule));
            }


            bool Repeat:: accepts(Y_Lingo_Syntax_Args) const
            {
                // checkRobustness("accepts()");
                // initializing
                XNode         *localTree = XNode::CreateFrom(*this);
                AutoPtr<XNode> guardTree = localTree;
                size_t         count     = 0;

                // accepting rule in local tree
                while( rule.accepts(lexer, source, localTree) ) {
                    assert(localTree->isWellFormed());
                    ++count;
                }

                if( count < atLeast )
                {
                    // return localTree to lexer
                    XNode::BackToLexer(lexer, guardTree.yield() );
                    return false;
                }
                else
                {
                    if(0==tree) {
                        // ill-formed grammar but produce something
                        tree = guardTree.yield();
                    }
                    else {
                        // steal content from localTree
                        assert(XNode::Internal==tree->type);
                        tree->fusion( localTree->branch() );
                    }
                    return true;
                }
            }
        }

    }

}
