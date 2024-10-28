
#include "y/lingo/lexical/scanner.hpp"
#include "y/lingo/pattern/char-db.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            void Scanner:: initialize() noexcept
            {
                for(unsigned i=0;i<CHARS;++i) new ( &rlist[i] ) RList();
            }


            Scanner:: ~Scanner() noexcept
            {
                unsigned i=CHARS;
                while(i-- > 0)
                    Destruct( &rlist[i] );
            }

            Scanner::ConstInterface &   Scanner:: surrogate() const noexcept
            {
                return rules;
            }

            void Scanner:: operator()(Rule * const rule)
            {
                assert(0!=rule);
                AutoPtr<Rule> ptr(rule);
                CharDB        cdb;
                Rule &        ref = *rule;
                rule->motif->query(cdb);
                try {

                    // register
                    for(unsigned i=0;i<CHARS;++i)
                        if( cdb.has(i) )
                            rlist[i] << ref;

                    // and finally append
                    rules.pushTail( ptr.yield() );
                }
                catch(...)
                {
                    for(unsigned i=0;i<CHARS;++i)
                        if( cdb.has(i) )
                            (void) rlist[i].no(ref);
                    throw;
                }
            }


            void Scanner:: run(Source        &source,
                               AutoPtr<Unit> &unit) const
            {
                assert(unit.isEmpty());

                // check EOF
                if(!source.ready()) {
                    std::cerr << "EOF" << std::endl;
                    return;
                }

                // analyze next byte in source
                const uint8_t byte = source.peek();
                const RList & auth = rlist[byte];


                if(auth.size<=0)
                    goto SYNTAX_ERROR; // no authorized rule


                {
                    const Rule * bestRule = 0;
                    Token        bestToken;
                    for(const RNode *node=auth.head;node;node=node->next)
                    {
                        const Rule &rule = **node;
                        if(rule.motif->takes(bestToken,source)) {
                            bestRule = & rule;
                            break;
                        }
                    }

                    if(!bestRule)
                        goto SYNTAX_ERROR;


                    std::cerr << "firstRule='" << bestRule->name << "'" << std::endl;
                    throw Exception("not implemented");
                }



                SYNTAX_ERROR:
                {
                    Token bad;
                    source.guess(bad);
                    assert(bad.size>0);
                    assert(0!=bad.head);
                    const String str = bad.toPrintable();
                    Specific::Exception excp(name->c_str(),"'%s' syntax error", str.c_str());
                    bad.head->stamp(excp);
                    throw excp;
                }

            }

        }

    }

}

