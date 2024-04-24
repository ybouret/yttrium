
#include "y/chemical/reactive/plexus/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            Warden:: Broken:: Broken(const Broken &other) noexcept :
            law(other.law),
            bad(other.bad),
            del(other.del)
            {
            }

            Warden:: Broken:: ~Broken() noexcept
            {
            }

            Warden:: Broken:: Broken(const Law         &L,
                                     const xreal_t      X,
                                     Writable<xreal_t> &D) noexcept :
            law(L), bad(X), del(D)
            {
            }
            
#if 1
            std::ostream & operator<<(std::ostream &os, const Warden::Broken &self)
            {
                os << self.law << " = " << real_t(self.bad);
                return os;
            }
#endif

            Warden:: Warden(const Clusters &clusters) :
            dC(clusters.maxCPG,clusters.maxSPG),
            repo(),
            jail(repo),
            xadd(clusters.maxSPG)
            {
                repo->reserve(clusters.maxCPG);
                std::cerr << "dC=" << dC << std::endl;
            }

            Warden:: ~Warden() noexcept
            {
            }

            static inline
            SignType BrokenCompare(const Warden::BrokenNode *lhs, const Warden::BrokenNode *rhs) noexcept
            {
                return Sign::Of( (**rhs).bad, (**lhs).bad );
            }

            void Warden:: process(Writable<xreal_t>  &C,
                                  Writable<xreal_t>  &I,
                                  const Group        &G,
                                  XMLog              &xml)
            {
                Y_XML_SECTION_OPT(xml, "Chemical::Warden::Group"," size='" << G->size << "'");
                static const xreal_t zero(0);
                const size_t         nlaw = G->size; assert(nlaw>0); assert(nlaw<=dC.rows);
                jail.free();                         assert(repo->stowage()>=nlaw);

                //--------------------------------------------------------------
                // init
                //--------------------------------------------------------------
                Y_XMLOG(xml, " (*) [initializing]");
                for(const LawNode *node=G->head;node;node=node->next)
                {
                    const Law         &law = **node;
                    Writable<xreal_t> &del = dC[jail.size+1];
                    const xreal_t      bad = law.required(del,AuxLevel, C, TopLevel, xadd);
                    if(bad>zero)
                    {
                        const Broken broken(law,bad,del);
                        jail << broken;
                        Y_XMLOG(xml, " (+) " << broken);
                    }
                    else
                    {
                        Y_XMLOG(xml, " (-) " << law);
                    }
                }

                //--------------------------------------------------------------
                // algo
                //--------------------------------------------------------------
                while(jail.size>0)
                {
                    Y_XMLOG(xml, " (*) [sorting " << jail.size << "]" );
                    //----------------------------------------------------------
                    // find greatest |dC|^2
                    //----------------------------------------------------------
                    MergeSort::Call(jail,BrokenCompare);
                    Y_XMLOG(xml, " (*) " << jail);
                    Broken &broken = **(jail.head);

                    //----------------------------------------------------------
                    // fix the broken law
                    //----------------------------------------------------------
                    Y_XMLOG(xml, " (*) apply " << broken);
                    for(const Actor *a=broken.law->head;a;a=a->next)
                    {
                        const Species &sp = a->sp;
                        const size_t   ii = sp.indx[TopLevel];
                        const xreal_t  dd = broken.del[sp.indx[AuxLevel]];
                        //std::cerr << sp << " + " << real_t(dd) << std::endl;
                        I[ii] += dd;
                        C[ii] += dd;
                        if(xml.verbose)
                        {
                            G.pad( xml() << " |_" << sp, sp )<< " += " << real_t(dd) << " = " << real_t(C[ii]) << std::endl;
                        }
                    }
                    //broken.bad = broken.law.required(broken.del, AuxLevel, C, TopLevel, xadd);
                    //std::cerr << "new bad=" << broken.bad << std::endl;
                    //----------------------------------------------------------
                    // remove it
                    //----------------------------------------------------------
                    jail.cutHead();

                    //----------------------------------------------------------
                    // check remaining by smart rotation
                    //----------------------------------------------------------
                    Y_XMLOG(xml, " (*) [updating]");
                    for(size_t i=jail.size;i>0;--i)
                    {
                        Broken &probe = **(jail.head);
                        probe.bad = probe.law.required(probe.del, AuxLevel, C, TopLevel,xadd);
                        if(probe.bad>zero)
                        {
                            Y_XMLOG(xml, " (+) " << probe);
                            jail.pushTail( jail.popHead() );
                        }
                        else
                        {
                            Y_XMLOG(xml, " (-) " << probe.law);
                            jail.cutHead();
                        }
                    }

                    //std::cerr << "remaining: " << jail.size << std::endl;

                }


            }

            void Warden:: operator()(Writable<xreal_t>  &C,
                                     Writable<xreal_t>  &I,
                                     const GList        &G,
                                     XMLog              &xml)
            {
                Y_XML_SECTION(xml, "Chemical::Warden");
                I.ld(0);
                for(const GNode *grp =G.head;grp;grp=grp->next)
                {
                    process(C,I,**grp,xml);
                }

            }

        }


    }

}


