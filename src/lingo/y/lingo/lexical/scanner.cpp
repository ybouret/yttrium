
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

                const Rule  * bestRule  = 0;
                Token         bestToken;
                {
                    // find first matching rule
                    RNode *node = auth.head;
                    for(;node;node=node->next)
                    {
                        const Rule &rule = **node;
                        std::cerr << "Probing '" << rule.name << "'" << std::endl;
                        if( rule.motif->takes(bestToken,source) )
                        {
                            bestRule = &rule;
                            if(bestToken.size<=0)
                                throw Specific::Exception(name->c_str(),"corrupted rule '%s'", bestRule->name->c_str());
                            break;
                        }
                    }

                    if(!bestRule)
                    {
                        assert(0==node);
                        return syntaxError(source,report);
                    }

                    assert(bestToken.size>0);
                    assert(bestRule == & **node);

                    std::cerr << "found " << bestRule->name << " = '" << bestToken << "'" << std::endl;

                    // find better rule
                    source.dup(bestToken);
                    for(node=node->next;node;node=node->next)
                    {
                        const Rule &rule = **node;
                        std::cerr << "Probing '" << rule.name << "'" << std::endl;
                        Token token;
                        if( rule.motif->takes(token,source) )
                        {
                            if(token.size>bestToken.size)
                            {
                                // new winner
                                bestToken.swapWith(token);
                                bestRule = &rule;
                                std::cerr << "new best '" << bestToken << "'" << std::endl;
                                source.dup(bestToken);
                            }
                            else
                            {
                                // too late
                                std::cerr << "too late '" << token << "'" << std::endl;
                                source.put(token);
                            }
                        }
                        else
                        {
                            // source is unchanged
                            assert(0==token.size);
                        }
                    }

                    // ok
                    source.skip( bestToken.size );

                }


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

