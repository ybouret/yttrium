
#include "y/chemical/weasel/formula/to-species.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        FormulaToSpecies:: ~FormulaToSpecies() noexcept
        {
        }

        FormulaToSpecies:: FormulaToSpecies(const Weasel::Parser &parser) :
        SPECIES("SPECIES", Lingo::Syntax::Terminal::Standard, Lingo::Syntax::Terminal::Semantic, 0),
        hashing(parser.actors)
        {

        }


        void FormulaToSpecies:: operator()(XTree &tree, Library &lib) const
        {
            XList &list = Coerce(tree->branch());
            for(XNode *node=list.head;node;node=node->next)
            {
                switch(hashing(node->name()))
                {
                    case Y_Weasel_REAC:
                    case Y_Weasel_PROD:
                        subst(*node,lib);
                        break;

                    default:
                        break;
                }
            }

        }

        void  FormulaToSpecies:: subst(XNode &parent, Library &lib) const
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
