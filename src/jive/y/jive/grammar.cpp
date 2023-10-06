#include "y/jive/grammar.hpp"
#include "y/type/nullify.hpp"
#include "y/system/exception.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Jive
    {


        typedef SuffixSet<String,Syntax::Rule::Pointer> Rules;

        class Grammar:: Code : public Object, public Rules
        {
        public:

            inline explicit Code(const Tag &id) :
            Object(),
            Rules(),
            name(*id),
            entry(0)
            {
            }

            inline virtual ~Code() noexcept
            {

            }

            void add(Rule *rule)
            {
                assert(0!=rule);

                const Rule::Pointer ptr(rule);
                if(!insert(ptr))
                    throw Specific::Exception(name.c_str(), "multiple rule [%s]",rule->name->c_str());
                if(!entry)
                    entry = rule;
            }

            inline void top(const Rule &rule)
            {
                const Rule::Pointer *ppR = search( *(rule.name) );
                if(!ppR) throw Specific::Exception(name.c_str(), "no rule [%s]", rule.name->c_str() );
                const Rule &mine = **ppR;
                if( &mine != &rule) throw Specific::Exception(name.c_str(), "foreign rule [%s]", rule.name->c_str() );
                entry = & Coerce(mine);
            }



            const String &name;
            Rule         *entry;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        Grammar::Code * Grammar::Initialize(const Tag &id)
        {
            return new Code(id);
        }
    }



}


namespace Yttrium
{
    namespace Jive
    {


        Grammar:: ~Grammar() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        void Grammar:: add(Rule *rule)
        {
            assert(0!=rule);
            assert(0!=code);
            code->add(rule);
        }

        void Grammar:: topLevel(const Rule &rule)
        {
            assert(0!=code);
            code->top(rule);
        }

        const Syntax::Rule & Grammar:: topLevel() const
        {
            assert(0!=code);
            if(0==code->entry) throw Specific::Exception( name->c_str(), "empty grammar, no top-level rule");
            return *(code->entry);
        }


        const Syntax::Rule & Grammar:: operator[](const String &ruleName) const
        {
            assert(0!=code);
            const Rule::Pointer *ppR = code->search( ruleName );
            if(!ppR) throw Specific::Exception( name->c_str(), "no rule [%s]", ruleName.c_str() );
            return **ppR;
        }

        const Syntax::Rule & Grammar:: ZOM(const Rule &rr)
        {
            return add( new Rep(rr,0) );
        }


        const Syntax::Rule & Grammar:: OOM(const Rule &rr)
        {
            return add( new Rep(rr,1) );
        }


        String * Grammar:: MakeNameFor(const Syntax::Manifest &m, const char sep)
        {
            AutoPtr<String> res = new String;
            {
                String         &s   = *res;
                s += '(';
                const Syntax::Manifest::NodeType *node = m.head;
                if(node)
                {
                    s += *((**node).name);
                    for(node=node->next;node;node=node->next)
                    {
                        s += sep;
                        s += *((**node).name);
                    }
                }
                s += ')';
            }
            return res.yield();
        }

    }

}

