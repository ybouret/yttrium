
#include "y/chemical/weasel/equilibrium/linker.hpp"
#include "y/chemical/weasel/parser.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equilibrium:: Linker:: ~Linker() noexcept
        {
        }

        Equilibrium:: Linker:: Linker(const Weasel::Parser &parser) :
        Lingo::Syntax::Translator(parser.EQUILIBRIUM.name),
        SPECIES("SPECIES", Lingo::Syntax::Terminal::Standard, Lingo::Syntax::Terminal::Semantic, 0),
        actors(parser.actors)
        {
            
        }


        void Equilibrium:: Linker:: preProcess(XTree &tree, Library &lib)
        {
            assert(*name == tree->name() );
            XList &list = Coerce(tree->branch());
            for(XNode *node=list.head;node;node=node->next)
            {
                switch(actors(node->name()))
                {
                    case Y_Weasel_REAC:
                    case Y_Weasel_PROD:
                        replaceFormulae(*node,lib);
                        break;

                    default:
                        break;
                }
            }

            GraphViz::Vizible::DotToPng("etree.dot", *tree);

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
                Lingo::Context         context(SPECIES.name,Lingo::AsCaption);
                AutoPtr<Lingo::Lexeme> lexeme = new Lingo::Lexeme(SPECIES,context);
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


