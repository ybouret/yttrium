
#include "y/chemical/plexus/initial/design.hpp"
#include "y/chemical/weasel.hpp"

#include "y/chemical/plexus/initial/axiom/electroneutrality.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            static inline
            void processInstruction(Design &       design,
                                    const String & instr,
                                    const Library &lib)
            {
                if(instr=="E/N")
                {
                    design.add( new ElectroNeutrality() );
                    return;
                }

                throw Specific::Exception(Design::CallSign, "unknown instruction '%s'", instr.c_str());

            }

            static inline
            void processFixedConcentration(Design        & design,
                                           const XList   & xlist,
                                           const Library & lib)
            {
                //new XNode(*xlist.head);
                const Formula formula(xlist.head);
            }

            Design:: Design(const Axioms   &axioms,
                            const Library  &lib,
                            const Clusters &cls) :
            Entity(axioms.name)
            {
                for(const XNode *node=axioms->head;node;node=node->next)
                {
                    const String &uuid = node->name();

                    if(uuid == Weasel::StringID) {
                        // this is an instruction
                        const String instr = node->lexeme().toString();
                        std::cerr << "New Instruction <" << instr << ">" << std::endl;
                        processInstruction(*this,instr,lib);
                        continue;
                    }

                    if(uuid == Design::_FixedConcentration)
                    {
                        processFixedConcentration(*this,node->branch(),lib);
                        continue;
                    }

                }
            }
        }
    }
}
