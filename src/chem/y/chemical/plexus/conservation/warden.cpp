
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
            xadd(),
            Cadd(act.species.size),
            Cini(act.species.size),
            Corr(act->size,act.species.size)
            {

                std::cerr << "Warden for #act=" << act->size << std::endl;
                for(const LNode *ln=act->head;ln;ln=ln->next)
                {
                    const Law & law = **ln;
                    enroll(*law);
                }
            }

          


            void Warden:: run(XMLog &xml, XWritable &C, const Level L)
            {
                Y_XML_SECTION_OPT(xml, CallSign, "size=" << act->size);

                const xReal zero;
                Cadd.forEach( & XAdd::free );
                Corr.ld(zero);
                blist.free();

                act.transfer(Cini, AuxLevel, C, L);
                for(const LNode *ln=act->head;ln;ln=ln->next)
                {
                    const Law & law = **ln;
                    const xReal xs  = law.excess(xadd,C,L);
                    if(xml.verbose)
                        print( xml(),  "d_(",*law, ")", Justify::Right) << " = " << xs.str() << std::endl;
                    if(xs<=0.0) continue;

                    XWritable &cc = Corr[blist.size+1];
                    std::cerr << "Cini=" << Cini << std::endl;
                    law.project(xadd,cc,Cini,AuxLevel);
                    std::cerr << "cc  =" << cc   << std::endl;
                }

            }


        }

    }

}

