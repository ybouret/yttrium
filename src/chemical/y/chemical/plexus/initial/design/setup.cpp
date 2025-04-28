
#include "y/chemical/plexus/initial/design.hpp"
#include "y/chemical/weasel.hpp"

#include "y/chemical/plexus/initial/axiom/electroneutrality.hpp"
#include "y/chemical/plexus/initial/axiom/fixed-concentration.hpp"
#include "y/chemical/plexus/initial/axiom/steady-conservation.hpp"

#include "y/lingo/pattern/matching.hpp"

#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            bool Design:: foundZeroConcentration(XMLog &xml,const Clusters &cls)
            {
                for(const SNode *sn=cls.witness.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn; if(defines(sp)) continue;
                    add( new FixedConcentration(sp,0) );
                    Y_XML_COMMENT(xml, "setting [" << sp << "]=0");
                    assert(defines(sp));
                    return true;
                }
                return false;
            }

            static inline
            void processInstruction(Design &       design,
                                    const String & instr,
                                    const Library &)
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

                const Formula  formula(new XNode(*xlist.head));
                const String   uuid  = formula.uuid();
                const String   value = xlist.tail->lexeme().toString();
                const Species &sp    = lib[uuid];
                const xreal_t  cc    = Weasel::Instance().eval(value);
                design.add( new FixedConcentration(sp,cc) );
            }

            static inline
            void processSteadyConservation(Design         & design,
                                           const XList    & xlist,
                                           const Clusters & cls)
            {
                const String                    expr = xlist.head->lexeme().toString();
                Lingo::Matching                 match(expr,0);
                const Conservation::Law * const law = cls.preserving(match);
                if(!law) throw Specific::Exception(Design::CallSign,"No Conservation matching '%s'", expr.c_str());
                const String                    value = xlist.tail->lexeme().toString();
                const xreal_t  cc = Weasel::Instance().eval(value);
                design.add( new SteadyConservation(*law,cc) );
            }


            bool Design:: defines(const Species &sp) const noexcept
            {
                for(const Axiom *a=my.head;a;a=a->next)
                {
                    if( FixedConcentration::UUID != a->uuid) continue;
                    if(a->contains(sp)) return true;
                }
                return false;
            }


            Design:: Design(const Axioms   &axioms,
                            const Library  &lib,
                            const Clusters &cls) :
            Entity(axioms.name),
            BaseType(),
            my()
            {
                // compile
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
                        // new fixed concentration
                        processFixedConcentration(*this,node->branch(),lib);
                        continue;
                    }


                    if(uuid == Design::_SteadyConservation)
                    {
                        // new steady conservation
                        processSteadyConservation(*this,node->branch(),cls);
                        continue;
                    }

                    throw Specific::Exception(CallSign,"unhandled Axiom '%s'", uuid.c_str());
                }


            }
        }
    }
}
