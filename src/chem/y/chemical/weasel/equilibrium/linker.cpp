
#include "y/chemical/weasel/equilibrium/linker.hpp"
#include "y/chemical/weasel/parser.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/system/exception.hpp"
#include "y/type/temporary.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equilibrium:: Linker:: ~Linker() noexcept
        {
        }

        Equilibrium:: Linker:: Linker(const Weasel::Parser &parser) :
        Lingo::Syntax::Translator(parser.EQUILIBRIUM.name),
        theLib(0),
        SPECIES("SPECIES", Lingo::Syntax::Terminal::Standard, Lingo::Syntax::Terminal::Semantic, 0),
        hashAct(parser.actors)
        {
            Y_Lingo_OnTerminal(Linker,EQ);
            Y_Lingo_OnTerminal(Linker,SPECIES);
            Y_Lingo_OnTerminal(Linker,INTEGER);
            Y_Lingo_OnTerminal(Linker,K);

            Y_Lingo_OnInternal(Linker,ACTOR);
            Y_Lingo_OnInternal(Linker,REAC);
            Y_Lingo_OnInternal(Linker,PROD);

        }

        void Equilibrium:: Linker:: onEQ(const Lingo::Lexeme &lexeme)
        {
            eqName = lexeme.toString(1,0);
            std::cerr << " (+) eqName='" << eqName << "'" << std::endl;
        }

        void Equilibrium:: Linker:: onSPECIES(const Lexeme &lexeme)
        {
            spName = lexeme.toString();
            std::cerr << " (+) spName='" << spName << "'" << std::endl;

        }

        void Equilibrium:: Linker:: onINTEGER(const Lexeme &lexeme)
        {
            static const char fn[] = "stoichiometry";
            const String    s = lexeme.toString();
            stoich = ASCII::Convert::To<unsigned>(s,fn);
            if(stoich<=0) throw Specific::Exception( name->c_str(), "forbidden zero %s",fn);
            std::cerr << " (+) stoich='" << stoich << "'" << std::endl;
        }

        void Equilibrium:: Linker:: onACTOR(const size_t n)
        {
            assert(spName.size()>0);
            assert(0!=theLib);

            switch(n)
            {
                case 1: assert(0==stoich); stoich=1; break;
                case 2: assert(stoich>0);  break;
                default:
                    // never get here
                    throw Specific::Exception( name->c_str(), "too many ACTOR args");
            }

            Library &lib  = *theLib;
            actors.pushTail( new Actor(stoich, lib(spName) ) );
            std::cerr << " (+) actors=" << actors << std::endl;

            stoich = 0;
            spName.free();
        }

        void Equilibrium::Linker:: onK(const Lexeme &lexeme)
        {
            Kstr = lexeme.toString(1,1);
            std::cerr << " (+) K=" << Kstr << std::endl;
        }


        void Equilibrium:: Linker:: onREAC(const size_t
#ifndef NDEBUG
                                           n
#endif
        )
        {
            assert(n==actors.size);
            assert(0==reac.size);
            reac.swapWith(actors);
            std::cerr << " (+) reac=" << reac << std::endl;
        }

        void Equilibrium:: Linker:: onPROD(const size_t
#ifndef NDEBUG
                                           n
#endif
        )
        {
            assert(n==actors.size);
            assert(0==prod.size);
            prod.swapWith(actors);
            std::cerr << " (+) prod=" << prod << std::endl;
        }


        void Equilibrium:: Linker::clear() noexcept
        {
            eqName.free();
            spName.free();
            stoich = 0;
            actors.release();
            reac.release();
            prod.release();
            Kstr.free();
        }

        void Equilibrium:: Linker:: init()
        {
            clear();
        }

        void Equilibrium:: Linker:: quit()
        {
            clear();
        }


        void Equilibrium:: Linker:: process(XTree &tree, Library &lib)
        {
            Lingo::Syntax::Translator &self = *this;
            theLib = 0;
            preProcess(tree,lib);
            const Temporary<Library *> tmpLib(theLib,&lib);
            self(*tree);
        }

        void Equilibrium:: Linker:: preProcess(XTree &tree, Library &lib)
        {
            assert(*name == tree->name() );
            XList &list = Coerce(tree->branch());
            for(XNode *node=list.head;node;node=node->next)
            {
                switch(hashAct(node->name()))
                {
                    case Y_Weasel_REAC:
                    case Y_Weasel_PROD:
                        replaceFormulae(*node,lib);
                        break;

                    default:
                        break;
                }
            }

            //GraphViz::Vizible::DotToPng("etree.dot", *tree);
        }


        void  Equilibrium:: Linker:: replaceFormulae(XNode &parent, Library &lib)
        {
            assert( "REAC" == parent.name() || "PROD" == parent.name() );

            //------------------------------------------------------------------
            //
            // loop over each ACTOR
            //
            //------------------------------------------------------------------
            for(XNode *actor=parent.branch().head;actor;actor=actor->next)
            {
                assert( "ACTOR" == actor->name() );

                //--------------------------------------------------------------
                // extract FORMULA tree
                //--------------------------------------------------------------
                XList &       content = actor->branch();   assert(content.size>0);
                XTree         tree    = content.popTail(); assert( "FORMULA" == tree->name() );
                Coerce(tree->sire) = 0;

                //--------------------------------------------------------------
                // convert it to species
                //--------------------------------------------------------------
                const Species &species = lib(tree);

                //--------------------------------------------------------------
                // make a SPECIES lexeme
                //--------------------------------------------------------------
                Lingo::Context  context(SPECIES.name,Lingo::AsCaption);
                AutoPtr<Lexeme> lexeme = new Lexeme(SPECIES,context);
                for(size_t i=1;i<=species.name.size();++i)
                {
                    context.newChar();
                    lexeme->pushTail( new Lingo::Char(context,species.name[i]) );
                }

                //--------------------------------------------------------------
                // replace into ACTOR
                //--------------------------------------------------------------
                actor->fusion( XNode::CreateFrom(SPECIES, lexeme.yield() ));
            }

        }


    }

}


