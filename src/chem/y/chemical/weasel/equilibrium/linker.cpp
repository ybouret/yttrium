
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
                std::cerr << "\t" << node->name() << std::endl;
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

        }


        void  Equilibrium:: Linker:: replaceFormulae(XNode &parent, Library &lib)
        {
            assert( "REAC" == parent.name() || "PROD" == parent.name() );
            for(XNode *actor=parent.branch().head;actor;actor=actor->next)
            {
                assert( "ACTOR" == actor->name() );
                XList &       content = actor->branch();                           assert(content.size>0);
                XTree         tree    = content.popTail(); Coerce(tree->sire) = 0; assert( "FORMULA" == tree->name() );
                const Formula formula(tree);
                const Species &species = lib.get(formula);
                
            }

        }


    }

}


