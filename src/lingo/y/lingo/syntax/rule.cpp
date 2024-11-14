
#include "y/lingo/syntax/rule.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            bool Rule:: Trace = false;

            Rule:: ~Rule() noexcept
            {
            }


            void Rule:: graphViz(OutputStream &fp) const
            {
                Enter(fp, "G");
                vizCode(fp);
                Leave(fp);
            }

            bool Rule:: flimsy() const
            {
                return !robust();
            }


            size_t XNode:: serialize(OutputStream &fp) const
            {
                size_t ans = rule.name->serialize(fp);
                switch(type)
                {
                    case Terminal:
                        // write lexeme content
                        ans += fp.emitVBR(lexeme().size);
                        for(const Char *ch = lexeme().head; ch; ch=ch->next)
                        {
                            fp.write( **ch );
                            ++ans;
                        }
                        break;

                    case Internal:
                        // write list
                        ans += fp.emitVBR( list().size );
                        for(const XNode *node=list().head;node;node=node->next)
                        {
                            ans += node->serialize(fp);
                        }
                        break;
                }

                return ans;
            }

            void Rule:: vizCode(OutputStream &fp) const
            {
                Node(fp,this) << '[';
                Label(fp,*name);
                vizMark(fp);
                Endl(fp << ']');
                vizLink(fp);
            }

            void Rule:: vizLink(OutputStream &) const
            {

            }

            void Rule:: vizArrow(OutputStream &fp, const Rule &r) const
            {
                Endl(Arrow(fp,this,&r));
            }


          
        }

    }
}


namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Rules:: ~Rules() noexcept
            {
            }

            Rules:: Rules() noexcept
            {
            }

            const Rule * Rules:: query(const Caption &rid) const noexcept
            {
                const String &lhs = *rid;
                for(const Rule *rule=head;rule;rule=rule->next)
                {
                    const String &rhs = *(rule->name);
                    if(lhs==rhs) return rule;
                }
                return 0;
            }

            const Rule * Rules:: query(const String &lhs) const noexcept
            {
                for(const Rule *rule=head;rule;rule=rule->next)
                {
                    const String &rhs = *(rule->name);
                    if(lhs==rhs) return rule;
                }
                return 0;
            }

            const Rule * Rules:: query(const char * const lhs) const noexcept
            {
                for(const Rule *rule=head;rule;rule=rule->next)
                {
                    const String &rhs = *(rule->name);
                    if(lhs==rhs) return rule;
                }
                return 0;
            }
        }

    }
}
