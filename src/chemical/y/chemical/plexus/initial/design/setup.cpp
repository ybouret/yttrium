
#include "y/chemical/plexus/initial/design.hpp"
#include "y/chemical/weasel.hpp"

#include "y/chemical/plexus/initial/axiom/electroneutrality.hpp"
#include "y/chemical/plexus/initial/axiom/fixed-concentration.hpp"
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
                std::cerr << "New Instruction <" << instr << ">" << std::endl;

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

                const Formula formula(new XNode(*xlist.head));
                const String  uuid = formula.uuid();
                const String  expr = xlist.tail->lexeme().toString();
                std::cerr << "New FixedConcentration [" << uuid << "] = '" << expr << "'" << std::endl;
                const Species &sp = lib[uuid];
                const xreal_t  cc = Weasel::Instance().eval(expr);
                design.add( new FixedConcentration(sp,cc) );
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
                        processInstruction(*this,instr,lib);
                        continue;
                    }

                    if(uuid == Design::_FixedConcentration)
                    {
                        processFixedConcentration(*this,node->branch(),lib);
                        continue;
                    }

                    throw Specific::Exception(CallSign,"unhandled Axiom '%s'", uuid.c_str());
                }
            }
        }
    }
}
