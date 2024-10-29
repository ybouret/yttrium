
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

            void Scanner:: add(Rule * const rule)
            {
                assert(0!=rule);
                AutoPtr<Rule> ptr(rule);
                for(const Rule *node=rules.head;node;node=node->next)
                {
                    if( *(node->name) == *(rule->name) )
                        throw Specific::Exception(name->c_str(), "multiple rule '%s'", node->name->c_str() );
                }

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


            Unit * Scanner:: produce(Token &token) const
            {
                assert(token.size>0);
                Unit * const unit =  new Unit(*this,*(token.head));
                unit->swapWith(token);
                return unit;
            }

            Unit * Scanner:: findError(Source   &source) const
            {

                // guess invalid token
                assert(source.ready());
                Token token;
                if(!source.guess(token)) throw Specific::Exception(name->c_str(),"corrupted source");
                assert(token.size>0);

                // convert to unit
                return produce(token);
            }



        }

    }

}


namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {


            Unit * Scanner:: run(Source  &source, Result &result) const

            {
                while(true)
                {
                    result = Regular;
                    if(!source.ready()) return 0; // 0+regular <=> EOF

                    //----------------------------------------------------------
                    //
                    //
                    // get authorized list of rules from next byte
                    //
                    //
                    //----------------------------------------------------------
                    const RList & auth     = rlist[ **source.peek() ];
                    const Rule  * bestRule = 0;
                    Token         bestToken;
                    {
                        //------------------------------------------------------
                        //
                        // find first matching rule
                        //
                        //------------------------------------------------------
                        RNode *node = auth.head;
                        for(;node;node=node->next)
                        {
                            const Rule &rule = **node;
                            std::cerr << "Probing '" << rule.name << "'" << std::endl;
                            if( rule.motif->takes(bestToken,source) )
                            {
                                bestRule = &rule;
                                if(bestToken.size<=0)
                                    throw Specific::Exception(name->c_str(),"corrupted rule '%s'", rule.name->c_str());
                                break;
                            }
                        }

                        if(!bestRule) {
                            assert(0==node);
                            result = Failure;
                            return findError(source); // unit+failure => syntax error
                        }

                        assert(bestToken.size>0);
                        assert(bestRule == & **node);

                        std::cerr << "found " << bestRule->name << " = '" << bestToken.toPrintable() << "'" << std::endl;

                        //------------------------------------------------------
                        //
                        // find better rule ?
                        //
                        //------------------------------------------------------

                        source.dup(bestToken); // reset source
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
                                    if(token.size<=0)
                                        throw Specific::Exception(name->c_str(),"corrupted rule '%s'", rule.name->c_str());
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
                    }

                    //----------------------------------------------------------
                    //
                    //
                    // process rule
                    //
                    //
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    // discard cached token
                    //----------------------------------------------------------
                    source.skip( bestToken.size );

                    //----------------------------------------------------------
                    // execute rule code
                    //----------------------------------------------------------
                    const Outcome outcome = bestRule->xcode(bestToken);

                    //----------------------------------------------------------
                    // check spot
                    //----------------------------------------------------------
                    switch(outcome.spot)
                    {
                        case Unit::Endl: source.newLine(); break;
                        case Unit::Bulk: break;
                    }

                    //----------------------------------------------------------
                    // check type
                    //----------------------------------------------------------
                    switch(outcome.type)
                    {
                        case Unit::Control:
                            result = Control;
                            return 0;   // 0+control => affect lexer

                        case Unit::Regular:
                            switch(outcome.args.regular.feat)
                            {
                                case Unit::Drop: continue; // silently drop token
                                case Unit::Emit: break;    // token->unit
                            }
                    }

                    //----------------------------------------------------------
                    // create and return unit
                    //----------------------------------------------------------
                    assert(Unit::Regular== outcome.type);
                    assert(Unit::Emit   == outcome.args.regular.feat);
                    AutoPtr<Unit> unit = new Unit(*bestRule,*bestToken.head);
                    unit->swapWith(bestToken);
                    return unit.yield();
                }
            }

        }

    }

}


