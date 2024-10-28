
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

                    // register in lists
                    for(unsigned i=0;i<CHARS;++i)
                        if( cdb.has(i) )
                            rlist[i] << ref;

                    // and finally append to rules
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


            Unit * Scanner:: run(Source   &source,
                                 unsigned &report) const

            {
                report = 0;
                if(!source.ready()) return 0;

                // get authorized list of rules from next byte
                const RList & auth = rlist[ **source.peek() ];

                // find first matching rule
                const Rule * bestRule  = 0;
                Token        bestToken;

                for(const RNode *node=auth.head;node;node=node->next)
                {
                    const Rule &rule = **node;
                    std::cerr << "Probing '" << rule.name << "'" << std::endl;
                    if(rule.motif->takes(bestToken,source))
                    {
                        bestRule = &rule;
                        std::cerr << "Accepted '" << bestToken << "'" << std::endl;
                        break;
                    }
                }

                if(!bestRule) return syntaxError(source,report);
                if(bestToken.size<=0) throw Specific::Exception(name->c_str(),"corrupted rule '%s'", bestRule->name->c_str());

                return 0;
            }

            Unit * Scanner:: syntaxError(Source   &source,
                                         unsigned &report) const
            {
                // mark report as error
                report = 1;

                // guess invalid token
                assert(source.ready());
                Token token;
                if(!source.guess(token)) throw Specific::Exception(name->c_str(),"corrupted source");
                assert(token.size>0);

                // convert to unit
                Unit * const unit =  new Unit(name,*(token.head));
                unit->swapWith(token);
                return unit;
            }



        }

    }

}

