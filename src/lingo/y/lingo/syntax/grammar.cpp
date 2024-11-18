
#include "y/lingo/syntax/grammar.hpp"
#include "y/system/exception.hpp"
#include "y/lingo/syntax/rule/visit.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Grammar:: ~Grammar() noexcept
            {
                
            }


            Grammar::ConstInterface & Grammar:: surrogate() const noexcept
            {
                return rules;
            }


            void Grammar:: add(Rule *const rule)
            {
                //--------------------------------------------------------------
                //
                // protect
                //
                //--------------------------------------------------------------
                assert(0!=rule);
                AutoPtr<Rule> ptr = rule; assert( ptr.isValid() );

                //--------------------------------------------------------------
                //
                // check unique name
                //
                //--------------------------------------------------------------
                {
                    const String &rid = *(rule->name);
                    if(locked) throw Specific::Exception(name->c_str(), "locked, trying to add '%s'", rid.c_str());

                    for(const Rule *mine=rules.head;mine;mine=mine->next)
                    {
                        const String &mid = *(mine->name);
                        if(mid==rid) throw Specific::Exception(name->c_str(),"multiple rule '%s'", rid.c_str());
                    }
                }

                //--------------------------------------------------------------
                //
                // append
                //
                //--------------------------------------------------------------
                rules.pushTail( ptr.yield() );
            }

            void Grammar:: throwMissing(const Caption &ruleName) const
            {
                throw Specific:: Exception(name->c_str(), "no Syntax::Rule '%s'", ruleName->c_str() );
            }

         

          


            void Grammar:: validate()
            {
                Coerce(locked) = false;

                if(rules.size<=0) throw Specific::Exception(name->c_str(), "no rule in validate()");

                const Rule & primary = *rules.head;
                {
                    const Visit visited(primary);
                    Manifest    orphans;
                    for(const Rule *node=primary.next;node;node=node->next)
                    {
                        const Rule &rule = *node;
                        if(!visited.has(rule)) orphans << rule;
                    }

                    if(orphans.size)
                    {
                        String res;
                        for(const RNode *rn=orphans.head;rn;rn=rn->next) {
                            res += ' ';
                            res += *(**rn).name;
                        }
                        throw Specific::Exception(name->c_str(),"orphaned%s",res.c_str());
                    }

                }

                Coerce(locked) = true;

            }


        }

    }

}

#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            void Grammar:: render() const
            {
                const String dotFile = *name + ".dot";
                {
                    OutputFile fp(dotFile);
                    GraphViz::Vizible::Enter(fp, "G");
                    for(const Rule *rule=rules.head;rule;rule=rule->next)
                    {
                        rule->vizCode(fp);
                    }
                    GraphViz::Vizible::Leave(fp);
                }
                GraphViz::Vizible::RenderPNG(dotFile,false);
            }
        }

    }

}

