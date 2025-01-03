
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

            Warden:: Warden(const Act & _act) :
            act(_act),
            xadd(),
            Cadd(act->species.size),
            Cini(act->species.size),
            Corr(act->size,act->species.size),
            blist(),
            extra(act->species.size),
            next(0),
            prev(0)
            {
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

                //--------------------------------------------------------------
                //
                //
                // initialize all
                //
                //
                //--------------------------------------------------------------
                const xReal zero;
                Cadd.forEach( & XAdd::free );
                Corr.ld(zero);
                Coerce(extra).ld(zero);
                blist.free();
                act.transfer(Cini,AuxLevel,C,L);

                if(xml.verbose) act( xml() << "Cini=", "\t[", Cini, "]", xReal::ToString) << std::endl;

                //--------------------------------------------------------------
                //
                //
                // first pass: populate broken laws
                //
                //
                //--------------------------------------------------------------
                {
                    Y_XML_SECTION(xml,"Inquiry");
                    for(const LNode *ln=act->head;ln;ln=ln->next)
                    {
                        const Law & law = **ln;
                        const xReal xs  = law.excess(xadd,C,L);
                        if(xs<=0.0) continue;
                        if(xml.verbose) print( xml(),  "d_(",*law, ")", Justify::Right) << " = " << xs.str() << std::endl;
                        XWritable &cc = Corr[blist.size+1];
                        law.project(xadd,cc,Cini,AuxLevel);
                        const Broken broken(law,xs,cc);
                        blist << broken;
                    }

                    if(blist.size<=0) {
                        Y_XML_COMMENT(xml, "clean state");
                        return;
                    }
                }

                //--------------------------------------------------------------
                //
                //
                // other passes: relived broken laws
                //
                //
                //--------------------------------------------------------------
                while(blist.size>0)
                {
                    Y_XML_SECTION_OPT(xml,"Relieve","broken=" << blist.size);

                    //----------------------------------------------------------
                    //
                    // sort in increasing excess
                    //
                    //----------------------------------------------------------
                    MergeSort::Call(blist,CompareBroken);
                    Y_XML_COMMENT(xml, "increasing excess");
                    for(const BNode *bn=blist.head;bn;bn=bn->next)
                    {
                        const Broken &broken = **bn;
                        if(xml.verbose)
                            print( xml(),  "d_(", *broken.law, ")", Justify::Right) << " = " << broken.bad.str() << std::endl;

                    }

                    //----------------------------------------------------------
                    //
                    // select least broken
                    //
                    //----------------------------------------------------------
                    {
                        const Broken    &least = **(blist.head);
                        const XReadable &Cnew  = least.cok;
                        Y_XML_COMMENT(xml, "using " << least.law);
                        if(xml.verbose)
                        {
                            act( xml() << "Cnew = ", "\t[", Cnew, "]", xReal::ToString) << std::endl;
                        }

                        // update concentrations and Cadd
                        for(const SNode *sn=act->species.head;sn;sn=sn->next)
                        {
                            const Species &sp    = **sn;
                            const xReal    c_old = sp(Cini,AuxLevel);
                            const xReal    c_new = sp(Cnew,AuxLevel); assert(c_new>=c_old);
                            const xReal    dekta = c_new-c_old;
                            if(c_new-c_old>0.0)
                                sp(Cadd,AuxLevel) << (c_new-c_old);
                            sp(Cini,AuxLevel) = c_new;
                        }
                    }


                    //----------------------------------------------------------
                    //
                    // remove fixed laws
                    //
                    //----------------------------------------------------------
                    Y_XML_COMMENT(xml, "remove fixed laws");
                    blist.cutHead();
                    for(BNode *node=blist.head;node;)
                    {
                        BNode * const next   = node->next;
                        Broken &      broken = **node;

                        if( broken.fixed(xadd,Cini) )
                        {
                            Y_XMLOG(xml, "[fixed] " << broken.law);
                            blist.cutNode(node);
                        }
                        else
                        {
                            Y_XMLOG(xml, "[still] " << broken.law);
                        }
                        node = next;
                    }

                    Y_XML_COMMENT(xml, "remaining #broken=" << blist.size);
                }

                //--------------------------------------------------------------
                //
                //
                // finalize
                //
                //
                //--------------------------------------------------------------
                for(const SNode *sn=act->species.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    const xReal    cc = sp(Cini,AuxLevel);
                    const xReal    dc = Coerce(sp(extra,AuxLevel)) = sp(Cadd,AuxLevel).sum();
                    if(xml.verbose) {
                        act->sformat.print( xml() << "[", sp, Justify::Right) << "] = " << cc.str() << " / " << sp(C,L).str() << " + " << dc.str() << std::endl;
                    }
                    sp(C,L) = cc;
                }

            }


        }

    }

}

