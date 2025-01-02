
#include "y/chemical/plexus/conservation/warden.hpp"

#include "y/chemical/plexus/conservation/act.hpp"
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        namespace Conservation
        {

            const char * const Warden:: CallSign = "Conservation::Warden";

            Warden:: ~Warden() noexcept
            {
            }

            Warden:: Warden(const Mix & _mix,
                            const Act & _act) :
            mix(_mix),
            act(_act),
            xadd()
            {

                std::cerr << "Warden for #act=" << act->size << std::endl;

            }

            void Warden:: run(XMLog &xml, XWritable &C, const Level L)
            {
                Y_XML_SECTION_OPT(xml, CallSign, "size=" << act->size);

                for(const LNode *ln=act->head;ln;ln=ln->next)
                {
                    const Law & law = **ln;
                    const xReal xs  = law.excess(xadd,C,L);
                    Y_XMLOG(xml, law << " => " << xs );
                }

            }


        }

    }

}

