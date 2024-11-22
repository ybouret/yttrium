
#include "y/lingo/syntax/grammar.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
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
        }
    }
}


namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
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

        }
    }
}

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

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

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            const Rule & Grammar:: cat(const Rule &a, const Rule &b)
            {
                Manifest tmp;
                tmp << a << b;
                return cat(tmp);
            }

            const Rule & Grammar:: cat(const Rule &a, const Rule &b, const Rule &c)
            {
                Manifest tmp;
                tmp << a << b << c;
                return cat(tmp);
            }

            const Rule & Grammar:: cat(const Rule &a, const Rule &b, const Rule &c, const Rule &d)
            {
                Manifest tmp;
                tmp << a << b << c << d;
                return cat(tmp);
            }

            const Rule & Grammar:: cat(const Manifest &tmp)
            {
                const String uid = Compound::MakeNameFor(tmp, ' ');
                return grp(uid) << tmp;
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
            Compound & Compound:: operator<<(const String & expr)
            {
                if(!host) { const Caption request(expr); noGrammarFor(request); }
                manifest << host->get(expr);
                return *this;
            }

            Compound & Compound:: operator<<(const char * const expr)
            {
                if(!host) { const Caption request(expr); noGrammarFor(request); }
                manifest << host->get(expr);
                return *this;
            }


            Compound & Compound:: operator<<(const char  expr)
            {
                if(!host) { const Caption request(expr); noGrammarFor(request); }
                manifest << host->get(expr);
                return *this;
            }



        }

    }

}

#include "y/lingo/parser.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            void Grammar:: noParserFor(const Caption &id) const
            {
                throw Specific::Exception(name->c_str(), "no host parser for '%s'", id->c_str());
            }

            const Rule & Grammar:: get_(const String &expr) {
                assert(0!=parser);
                return parser->get(expr);
            }
            
            const Rule & Grammar:: get_(const char * const expr) {
                assert(0!=parser);
                return parser->get(expr);
            }

            const Rule & Grammar:: get_(const char expr) {
                assert(0!=parser);
                return parser->get(expr);
            }
        }

    }

}


