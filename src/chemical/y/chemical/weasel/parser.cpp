
#include "y/chemical/weasel/parser.hpp"
#include "y/lingo/lexical/add-on/single-line-comment.hpp"
#include "y/lingo/lexical/add-on/multi-lines-comment.hpp"
#include "y/lingo/lexical/add-on/rstring.hpp"

#include "y/system/exception.hpp"
#include <iomanip>

#include "y/apex/natural.hpp"

#include "y/chemical/plexus/initial/wish.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        Weasel::Parser:: ~Parser() noexcept
        {
        }


        Weasel:: Parser:: Parser() : Lingo::Parser("Weasel")
        {
            //------------------------------------------------------------------
            //
            // Top Level will always be Weasel
            //
            //------------------------------------------------------------------
            Agg &WEASEL    = agg(CallSign);
            Alt &STATEMENT = alt("STATEMENT");

            //------------------------------------------------------------------
            //
            // Whitespaces
            //
            //------------------------------------------------------------------
            const Rule &WHITE     = alt("WHITE") << endl("ENDL","[:endl:]",Dividing) << mark("BLANK", "[ \t]");
            const Rule &SEPARATOR = mark(';');
            const Rule &OPT_SEP   = opt(SEPARATOR);
            WEASEL << zom(STATEMENT);

            STATEMENT << SEPARATOR;                // single separator
            STATEMENT << cat(oom(WHITE),OPT_SEP);  // blank line

            //------------------------------------------------------------------
            //
            // Common rules for Equilibria/Formula
            //
            //------------------------------------------------------------------
            const Rule &COEF      = term(Coef,"[:digit:]+");
            const Rule &OPT_COEF  = opt(COEF);
            Agg        &FORMULA   = agg(Formula::CallSign);
            const Rule &PLUS      = term('+');
            const Rule &MINUS     = term('-');
            const Rule &SIGN      = pick(PLUS,MINUS);
            const Rule &SPACE     = zom(WHITE);
            const Rule &STRING    = plug<Lingo::Lexical::RString>(StringID);


            //------------------------------------------------------------------
            //
            // Create Formula
            //
            //------------------------------------------------------------------
            {
                const Rule &NAME = term(Formula::Name,"[:upper:][[:lower:]_]*");
                Agg        &MULT = act(Formula::Mult);
                const Rule &BODY = act(Formula::Body) << MULT << zom(MULT);
                Alt        &ITEM = alt("Item");
                MULT    << ITEM     << OPT_COEF;
                ITEM    << NAME     << parens(BODY);
                FORMULA << BODY << opt(agg(Formula::Z) << '^' << OPT_COEF << SIGN);
            }
            STATEMENT << FORMULA;

            //------------------------------------------------------------------
            //
            // Create Equilibrium
            //
            //------------------------------------------------------------------
            Agg &EQUILIBRIUM = agg(Equilibrium::CallSign);
            {
                String      rx    = Equilibrium::Prefix; rx +="[[:word:]_\\(\\)]+";
                const Rule &LABEL = term(Equilibrium::Label,rx);
                const Rule &EQSEP = mark(Equilibrium::Separator);
                EQUILIBRIUM << LABEL;
                EQUILIBRIUM << SPACE << EQSEP;

                const Rule & ACTOR  = agg(Actor::CallSign)  << SPACE << OPT_COEF << SPACE << FORMULA << SPACE;
                const Rule & ACTORS = grp(Actors::CallSign) << ACTOR << extra('+',ACTOR);
                const Rule & EQSIDE = pick(ACTORS,SPACE);
                EQUILIBRIUM << (agg(Equilibrium::Reac) << EQSIDE);
                EQUILIBRIUM << SPACE << mark(Equilibrium::Symbol);
                EQUILIBRIUM << (agg(Equilibrium::Prod) << EQSIDE);

                EQUILIBRIUM << SPACE << EQSEP;
                EQUILIBRIUM << SPACE << STRING;
            }

            STATEMENT << EQUILIBRIUM;

            //------------------------------------------------------------------
            //
            // Create Regular Expression
            //
            //------------------------------------------------------------------
            STATEMENT<< term(RegExp,"%[-+[:word:].*?\\\\\\(\\)&|]+");;


            //------------------------------------------------------------------
            //
            // Create Wish
            //
            //------------------------------------------------------------------
            {
                Agg &WISH = agg(Wish::CallSign);
                WISH << '#';

                Agg &LINEAR_WISH = agg("LinearWish");

                LINEAR_WISH << SPACE << '=' << SPACE << STRING;

                WISH << pick(LINEAR_WISH,STRING);

                STATEMENT << WISH;
            }

            //------------------------------------------------------------------
            // Lexical Only
            //------------------------------------------------------------------

            (void) lexer.plug<Lingo::Lexical::CPlusPlusComment>("Comment++");
            (void) lexer.plug<Lingo::Lexical::C_Comment>("Comment");
            render();
        }



        static inline void cleanupFormula(XNode * const formula) noexcept
        {
            assert(formula->is(Formula::CallSign));
            XList &list = formula->branch(); assert(1==list.size||2==list.size);
            if(2==list.size)
            {
                XNode * const zNode = list.tail; assert(Formula::Z==zNode->name());
                XList        &zlist = zNode->branch();
                if(2==zlist.size)
                {
                    XNode * const coefNode = zlist.head; assert(Weasel::Coef==coefNode->name());
                    const Lexeme &coefLexm = coefNode->lexeme();
                    const apn     coef     = coefLexm.toNatural();

                    if(1==coef)
                    {
                        // removing unit coef
                        delete zlist.popHead();
                        return;
                    }

                    if(0==coef)
                    {
                        // removing charge
                        delete list.pop(zNode);
                        return;
                    }
                }
            }
        }


        static inline void cleanupActors(XNode * const acNode) noexcept
        {
            assert(0!=acNode);
            assert( acNode->is(Equilibrium::Reac) || acNode->is(Equilibrium::Prod) );
            XList &list = acNode->branch();
            XList  temp;

            while(list.size>0)
            {
                XNode * node = list.popHead();
                if( '+' == node->name() )
                {
                    delete node;
                    continue;
                }

                if( Actor::CallSign == node->name() )
                {
                    XList &list = node->branch(); assert(Formula::CallSign == list.tail->name() );
                    cleanupFormula(list.tail);
                    if(2==list.size)
                    {
                        assert(Weasel::Coef == list.head->name() );
                        const apn sto = list.head->lexeme().toNatural();

                        if(1==sto)
                        {
                            // remove coefficient
                            delete list.popHead();
                            goto KEEP;
                        }

                        if(0==sto)
                        {
                            // remove actor
                            delete node;
                            continue;
                        }
                    }
                }

            KEEP:
                temp.pushTail(node);
            }
            list.swapWith(temp);
        }


        static inline void cleanupString(XNode * const node) noexcept
        {
            assert(node->is(Weasel::StringID));
            Lingo::Token &token = node->lexeme(); assert(token.size>=2);
            delete token.popTail();
            delete token.popHead();
            token.crop(isspace);
        }

        static inline void cleanupEquilibrium(XNode * const node) noexcept
        {
            assert(0!=node);
            assert( node->is(Equilibrium::CallSign) );
            XList &list = node->branch(); assert(4==list.size);
            {
                XNode * const reac = list.fetch(2); assert( reac->is(Equilibrium::Reac)   );
                cleanupActors(reac);
            }

            {
                XNode * const prod = list.fetch(3); assert( prod->is(Equilibrium::Prod) );
                cleanupActors(prod);
            }

            cleanupString(list.tail);
        }


        XNode * Weasel::Parser:: preprocess(Lingo::Module * const inputModule)
        {
            Lingo::Parser &self = *this;
            AutoPtr<XNode> ast  = self(inputModule);

            assert( CallSign == ast->name() );


            {
                XList &mine = ast->branch();
                XList  temp;

                while(mine.size>0)
                {
                    assert(0!=mine.head);
                    AutoPtr<XNode> node = mine.popHead();

                    if( node->is(Equilibrium::CallSign) )
                    {
                        cleanupEquilibrium(& *node);
                        goto PUSH;
                    }

                    if( node->is(Formula::CallSign) )
                    {
                        cleanupFormula( & *node);
                        goto PUSH;
                    }


                    if( node->is(RegExp) )
                    {
                        const String regexp = '@' + node->lexeme().toString(1,0);
                        processRegExp(temp,regexp);
                        continue; // drop node
                    }

                    std::cerr << "unprocessed " << node->name() << std::endl;

                PUSH:
                    temp.pushTail(node.yield());

                }

                mine.swapWith(temp);

            }

            GraphViz::Vizible::DotToPng("ast.dot", *ast);

            return ast.yield();
        }






    }

}

#include "y/lingo/pattern/matching.hpp"
#include "y/chemical/weasel/equilibrium/db.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/container/algo/crop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Weasel:: Parser:: processRegExp(XList &       target,
                                             const String &regexp)
        {
            Lingo::Matching               matching(regexp);
            Vector<String,Memory::Pooled> words(4,AsCapacity);
            size_t                        count = 0;
            for(size_t i=0;i<EDB::Count;++i)
            {
                words.free();
                const String entry = EDB::Table[i];
                Tokenizer::AppendTo(words,entry,':');
                if(words.size()!=3) throw Specific::Exception(CallSign, "invalid EDB[%u]", unsigned(i) );
                String &uuid = words[1];
                Algo::Crop(uuid,isspace);
                if(matching.exactly(uuid,uuid))
                {
                    ++count;
                    //std::cerr << "Matching '" << uuid << "'" << std::endl;
                    AutoPtr<XNode> root = preprocess( Lingo::Module::OpenData(uuid,entry) ); assert( root->is(CallSign) );
                    target.mergeTail(root->branch());
                }

            }

            if(count<=0) throw Specific::Exception(CallSign,"no matching '%s'", regexp.c_str());
        }

    }

}

