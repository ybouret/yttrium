
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

            void Scanner:: expunge_(const Rule &rule) const noexcept
            {
                for(unsigned i=0;i<CHARS;++i) rlist[i].no(rule);
            }

            void Scanner:: indorse_(Rule &rule, const CharDB &cdb)
            {
                for(unsigned i=0;i<CHARS;++i)
                    if( cdb.has(i) )
                        rlist[i] << rule;
            }

            const Rule & Scanner:: add(Rule * const rule)
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
                    indorse_(ref,cdb);
                    return *rules.pushTail( ptr.yield() );
                }
                catch(...) { expunge_(ref); throw; }
            }


            void Scanner:: cut(const Rule & rule) noexcept
            {
                assert(rules.owns(&rule));
                expunge_(rule);
                delete rules.pop( & Coerce(rule) );
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
                //--------------------------------------------------------------
                //
                // guess invalid token
                //
                //--------------------------------------------------------------
                assert(source.ready());
                Token token;
                if(!source.guess(token)) throw Specific::Exception(name->c_str(),"corrupted source");
                assert(token.size>0);

                //--------------------------------------------------------------
                //
                // convert to unit
                //
                //--------------------------------------------------------------
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


            Unit * Scanner:: run(Source  &source, Result &result)

            {
                while(true)
                {
                    result = Regular;
                    if(!source.ready())
                        return 0; // 0+regular => EOF

                    //----------------------------------------------------------
                    //
                    //
                    // get authorized list of rules from next byte
                    //
                    //
                    //----------------------------------------------------------
                    assert(source.ready());
                    assert(0!=source.peek());
                    const RList & auth     = rlist[ **source.peek() ];
                    Rule  *       bestRule = 0;
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
                            Rule &rule = **node;
                            if( rule.motif->takes(bestToken,source,0) )
                            {
                                bestRule = &rule;
                                if(bestToken.size<=0)
                                    throw Specific::Exception(name->c_str(),"corrupted rule '%s'", rule.name->c_str());
                                break;
                            }
                        }

                        if(!bestRule) {
                            assert(0==node);          // tried all possibilities
                            result = Failure;         // fill in result
                            return findError(source); // unit+failure => syntax error
                        }

                        assert(bestToken.size>0);
                        assert(bestRule == & **node);

                        //------------------------------------------------------
                        //
                        // find better rule ?
                        //
                        //------------------------------------------------------

                        source.dup(bestToken); // reset source for next trials
                        for(node=node->next;node;node=node->next)
                        {
                            Rule &rule = **node;
                            Token token;
                            if( rule.motif->takes(token,source,0) )
                            {
                                if(token.size>bestToken.size)
                                {
                                    // new winner
                                    bestToken.swapWith(token);
                                    bestRule = &rule;
                                    source.dup(bestToken);
                                }
                                else
                                {
                                    // too late
                                    if(token.size<=0)
                                        throw Specific::Exception(name->c_str(),"corrupted rule '%s'", rule.name->c_str());
                                    source.put(token);
                                }
                            }
                            else
                            {
                                // source is unchanged, ready for next trial
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
                    assert(bestToken.size>0);
                    assert(source.cached()>=bestToken.size);
                    
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
                            return 0;   // 0+control => affect Lexer

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


