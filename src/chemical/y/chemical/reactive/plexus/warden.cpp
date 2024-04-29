
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
            cok(other.cok),
            pad(other.pad)
            {
            }

            Warden:: Broken:: ~Broken() noexcept
            {
            }

            Warden:: Broken:: Broken(const Law         &L,
                                     const xreal_t      X,
                                     Writable<xreal_t> &C,
                                     const size_t       p) noexcept :
            law(L), bad(X), cok(C), pad(p)
            {
            }
            
#if 1
            std::ostream & operator<<(std::ostream &os, const Warden::Broken &self)
            {
                //os << std::setw(15) << real_t(self.bad) << " @"  << self.law;
                os << self.law;
                for(size_t i=self.law.uuid.size();i<=self.pad;++i) os << ' ';
                os <<  " = " << std::setw(15) << real_t(self.bad) << " @" << self.cok;
                return os;
            }
#endif

            Warden:: Warden(const Clusters &clusters) :
            Caux(clusters.maxCPG,clusters.maxSPG),
            repo(),
            jail(repo),
            xadd(clusters.maxSPG)
            {
                repo->reserve(clusters.maxCPG);
                std::cerr << "Caux=" << Caux << std::endl;
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
                assert( G->size>0 );
                assert( G->size<=Caux.rows);
                assert(repo->stowage()>=G->size);

                //--------------------------------------------------------------
                // init with all unbalanced laws
                //--------------------------------------------------------------
                Y_XMLOG(xml, " (*) [initializing]");
                jail.free();
                for(const LawNode *node=G->head;node;node=node->next)
                {
                    const Law         &law = **node;
                    Writable<xreal_t> &cok = Caux[jail.size+1];
                    const xreal_t      bad = law.required(cok,C,xadd);
                    if(bad>zero)
                    {
                        const Broken broken(law,bad,cok,G.maxUUID);
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
                const bool injected = jail.size>0;
                while(jail.size>0)
                {
                    Y_XMLOG(xml, " (*) [sorting #" << jail.size << "]" );
                    //----------------------------------------------------------
                    // find greatest |dC|^2
                    //----------------------------------------------------------
                    MergeSort::Call(jail,BrokenCompare);
                    for(const BrokenNode *node=jail.head;node;node=node->next)
                    {
                        const Broken &bk = **node;
                        Y_XMLOG(xml, "  |- " << bk);

                    }
                    //Y_XMLOG(xml, " (*) " << jail);
                    Broken &broken = **(jail.head);

                    //----------------------------------------------------------
                    // fix the broken law
                    //----------------------------------------------------------
                    Y_XMLOG(xml, " ($) " << broken);
                    for(const Actor *a=broken.law->head;a;a=a->next)
                    {
                        const Species &sp = a->sp;
                        const size_t   ii = sp.indx[TopLevel];
                        const xreal_t  cc = broken.cok[sp.indx[AuxLevel]];
                        //std::cerr << sp << " + " << real_t(dd) << std::endl;
                        const xreal_t  c0 = C[ii];
                        I[ii] += cc - c0;
                        C[ii]  = cc;
                        if(xml.verbose)
                        {
                            G.pad( xml() << "  |_" << sp, sp )<< " = " << std::setw(15) << real_t(C[ii]) << " <- " << real_t(c0) << std::endl;
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
                        probe.bad = probe.law.required(probe.cok, AuxLevel, C, TopLevel,xadd);
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
                    Y_XMLOG(xml, " (*) [remaining #" << jail.size << "]");

                    //std::cerr << "remaining: " << jail.size << std::endl;

                }

                if(injected)
                {
                    Y_XMLOG(xml, " (*) [modified]");
                    
                }
                else
                {
                    Y_XMLOG(xml, " (*) [everything was ok]");
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


