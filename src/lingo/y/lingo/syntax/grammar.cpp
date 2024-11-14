
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
                // check no multiple name
                assert(0!=rule);
                AutoPtr<Rule> pRule = rule; assert( pRule.isValid() );
                const String &rid   = *(rule->name);
                if(locked) throw Specific::Exception(name->c_str(), "locked, trying to add '%s'", rid.c_str());

                for(const Rule *mine=rules.head;mine;mine=mine->next)
                {
                    const String &mid = *(mine->name);
                    if(mid==rid) throw Specific::Exception(name->c_str(),"multiple rule '%s'", rid.c_str());
                }

                // append
                rules.pushTail( pRule.yield() );
            }

            const Rule & Grammar:: top() const
            {
                if(rules.size<=0) throw Specific::Exception(name->c_str(), "no rule in top()");
                return *(rules.head);
            }

            void Grammar:: top(const Rule &rule)
            {
                if(locked)                 throw Specific::Exception(name->c_str(), "locked, trying to top('%s')", rule.name->c_str());
                if( !rules.owns( &rule ) ) throw Specific::Exception(name->c_str(), "unknown rule for top('%s')", rule.name->c_str());
                rules.moveToFront( &Coerce(rule) );
            }

            void Grammar:: no(const String &ruleName) noexcept
            {
                assert(!locked);
                Rules store;
                while(rules.size>0)
                {
                    AutoPtr<Rule> rule = rules.popHead();
                    if( ruleName != *(rule->name) ) store.pushTail( rule.yield() );
                }
                rules.swapWith(store);
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

            


            const Rule & Grammar:: zom(const Rule &other)
            {
                if( !rules.owns(&other) ) throw Specific::Exception(name->c_str(),"unknown rule '%s' in zero-or-more", other.name->c_str());
                return decl( new Repeat(other,0) );
            }

            const Rule & Grammar:: oom(const Rule &other)
            {
                if( !rules.owns(&other) ) throw Specific::Exception(name->c_str(),"unknown rule '%s' in one-or-more", other.name->c_str());
                return decl( new Repeat(other,1) );
            }

            const Rule & Grammar:: rep(const Rule &other, const size_t n)
            {
                if( !rules.owns(&other) ) throw Specific::Exception(name->c_str(),"unknown rule '%s' in repeat>=%u", other.name->c_str(), unsigned(n));
                return decl( new Repeat(other,n) );
            }


            const Rule & Grammar:: opt(const Rule &other)
            {
                if( !rules.owns(&other) ) throw Specific::Exception(name->c_str(),"unknown rule '%s' as option", other.name->c_str() );
                return decl( new Option(other) );
            }


            const Rule & Grammar:: pick(const Rule &a, const Rule &b)
            {
                Manifest tmp;
                tmp << a << b;
                return pick(tmp);
            }

            const Rule & Grammar:: pick(const Rule &a, const Rule &b, const Rule &c)
            {
                Manifest tmp;
                tmp << a << b << c;
                return pick(tmp);
            }

            const Rule & Grammar:: pick(const Rule &a, const Rule &b, const Rule &c, const Rule &d)
            {
                Manifest tmp;
                tmp << a << b << c << d;
                return pick(tmp);
            }

            const Rule & Grammar:: pick(const Manifest &tmp)
            {
                const String uid = Compound::MakeNameFor(tmp, '|');
                return alt(uid) << tmp;
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

