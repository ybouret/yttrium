
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
            Cadd(act->species.size),
            Cini(act->species.size),
            Corr(act->size,act->species.size)
            {

                std::cerr << "Warden for #act=" << act->size << std::endl;
                for(const LNode *ln=act->head;ln;ln=ln->next)
                {
                    const Law & law = **ln;
                    enroll(*law);
                }
            }

          
            static inline SignType CompareBroken(const BNode * const lhs,
                                                 const BNode * const rhs) noexcept
            {
                const Broken &L = **lhs;
                const Broken &R = **rhs;
                return Sign::Of(L.bad,R.bad);
            }

            void Warden:: run(XMLog &xml, XWritable &C, const Level L)
            {
                Y_XML_SECTION_OPT(xml, CallSign, "size=" << act->size << " species=" << act->species);

                // initialize all
                const xReal zero;
                Cadd.forEach( & XAdd::free );
                Corr.ld(zero);
                blist.free();


                act.transfer(Cini,AuxLevel,C,L);
                if(xml.verbose) act( xml() << "Cini=", "\t[", Cini, "]", xReal::ToString) << std::endl;

                {
                    Y_XML_SECTION(xml,"Inquiry");
                    for(const LNode *ln=act->head;ln;ln=ln->next)
                    {
                        const Law & law = **ln;
                        const xReal xs  = law.excess(xadd,C,L);
                        if(xml.verbose) print( xml(),  "d_(",*law, ")", Justify::Right) << " = " << xs.str() << std::endl;
                        if(xs<=0.0) continue;

                        XWritable &cc = Corr[blist.size+1];
                        law.project(xadd,cc,Cini,AuxLevel);
                        //if(xml.verbose) act( xml() << "Corr=","  [", cc, "]", xReal::ToString) << std::endl;
                        const Broken broken(law,xs,cc);
                        blist << broken;
                    }
                }

                while(blist.size>0)
                {
                    Y_XML_SECTION_OPT(xml,"Relieve","broken=" << blist.size);

                    MergeSort::Call(blist,CompareBroken);
                    Y_XML_COMMENT(xml, "increasing excess");
                    for(const BNode *bn=blist.head;bn;bn=bn->next)
                    {
                        const Broken &broken = **bn;
                        if(xml.verbose)
                        {
                            print( xml(),  "d_(", *broken.law, ")", Justify::Right) << " = " << broken.bad.str() << std::endl;
                        }
                    }

                    const Broken    &broken = **(blist.head);
                    const XReadable &Cnew   = broken.cok;
                    Y_XML_COMMENT(xml, "using " << broken.law);
                    if(xml.verbose)
                    {
                        act( xml() << broken.law << " => ","  [", Cnew, "]", xReal::ToString) << std::endl;
                    }
                    for(const SNode *sn=act->species.head;sn;sn=sn->next)
                    {
                        const Species &sp    = **sn;
                        const xReal    c_old = sp(Cini,AuxLevel);
                        const xReal    c_new = sp(Cnew,AuxLevel); assert(c_new>=c_old);
                        sp(Cadd,AuxLevel) << (c_new-c_old);
                        sp(Cini,AuxLevel) = c_new;
                    }
                    std::cerr << "Cadd=" << Cadd << std::endl;


                    break;
                }


            }


        }

    }

}

