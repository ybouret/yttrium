
#include "y/chemical/weasel/parser.hpp"
#include "y/lingo/lexical/add-on/single-line-comment.hpp"
#include "y/lingo/lexical/add-on/multi-lines-comment.hpp"
#include "y/lingo/lexical/add-on/rstring.hpp"

#include "y/system/exception.hpp"
#include <iomanip>

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
            const Rule &STRING    = plug<Lingo::Lexical::RString>("String");


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
                String rx = Equilibrium::Prefix; rx +="[[:word:]_\\(\\))]+";
                const Rule &LABEL = term("Label",rx);
                const Rule &EQSEP = mark(Equilibrium::Separator);
                EQUILIBRIUM << LABEL;
                EQUILIBRIUM << SPACE << EQSEP;

                const Rule & ACTOR  = agg("Actor")  << SPACE << OPT_COEF << SPACE << FORMULA << SPACE;
                const Rule & ACTORS = grp("Actors") << ACTOR << extra('+',ACTOR);
                const Rule & EQSIDE = pick(ACTORS,SPACE);
                EQUILIBRIUM << (agg(Equilibrium::Reac) << EQSIDE);
                EQUILIBRIUM << SPACE << mark(Equilibrium::Symbol);
                EQUILIBRIUM << (agg(Equilibrium::Prod) << EQSIDE);

                EQUILIBRIUM << SPACE << EQSEP;
                EQUILIBRIUM << SPACE << STRING;
            }

            STATEMENT << EQUILIBRIUM;

            //------------------------------------------------------------------
            // Lexical Only
            //------------------------------------------------------------------

            (void) lexer.plug<Lingo::Lexical::CPlusPlusComment>("Comment++");
            (void) lexer.plug<Lingo::Lexical::C_Comment>("Comment");
            render();
        }



        static inline void cleanupActors(XNode * const actors) noexcept
        {
            assert( Equilibrium::Reac == actors->name() || Equilibrium::Prod == actors->name() );
            XList &list = actors->branch();
            XList  temp;

            while(list.size>0)
            {
                XNode * node = list.popHead();
                if( '+' == node->name() )
                {
                    //std::cerr << "Removing " << node->name() << std::endl;
                    delete node;
                    continue;
                }
                temp.pushTail(node);
            }
            list.swapWith(temp);
        }

        static inline void cleanupFormula(XNode * const formula) noexcept
        {
            std::cerr << "cleanupFormula" << std::endl;
            assert( Formula::CallSign == formula->name() );
            XList &list = formula->branch(); assert(1==list.size||2==list.size);
            if(2==list.size)
            {
                XNode * const zNode = list.tail; assert(Formula::Z==zNode->name());
                XList        &zlist = zNode->branch();
                if(2==zlist.size)
                {
                    XNode * const coefNode = zlist.head; assert(Weasel::Coef==coefNode->name());
                    const Lexeme &coefLexm = coefNode->lexeme();
                    std::cerr << "Coef=" << coefLexm << std::endl;
                }
            }
        }

        XNode * Weasel::Parser:: preprocess(Lingo::Module * const inputModule)
        {
            Lingo::Parser &self = *this;
            AutoPtr<XNode> ast  = self(inputModule);

            assert( CallSign == ast->name() );

            for(XNode *node=ast->branch().head;node;node=node->next)
            {
                std::cerr << "Got '" << node->name() << "'" << std::endl;

                if( Equilibrium::CallSign == node->name() )
                {
                    XList &list = node->branch(); assert(4==list.size);
                    {
                        XNode * const reac = list.fetch(2); assert( Equilibrium::Reac == reac->name() );
                        cleanupActors(reac);
                    }
                    {
                        XNode * const prod = list.fetch(3); assert( Equilibrium::Prod == prod->name() );
                        cleanupActors(prod);
                    }
                    continue;
                }

                if( Formula::CallSign == node->name() )
                {
                    cleanupFormula(node);
                    continue;
                }


            }


            return ast.yield();
        }


        

    }

}
