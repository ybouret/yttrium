
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
                os <<  " = " << std::setw(15) << real_t(self.bad) << " @";
                os << '[';
                os << real_t(self.cok[1]);
                for(size_t j=2;j<=self.cok.size();++j) os << ';' << real_t(self.cok[j]);
                os << ']';
                return os;
            }
#endif

            Warden:: Warden(const Clusters &clusters) :
            Caux(clusters.maxCPG,clusters.maxSPG),
            repo(),
            jail(repo),
            xadd(clusters.maxSPG),
            xinj(clusters.maxSPG)
            {
                repo->reserve(clusters.maxCPG);
                for(size_t i=1;i<=clusters.maxSPG;++i)
                {
                    xinj[i].make(clusters.maxCPG); //each law can modify at most once the injection
                }
            }

            Warden:: ~Warden() noexcept
            {
            }

            static inline
            SignType BrokenCompare(const Warden::BrokenNode *lhs, const Warden::BrokenNode *rhs) noexcept
            {
                return Sign::Of( (**lhs).bad, (**rhs).bad );
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
                // setup Injected and injectors
                //--------------------------------------------------------------
                for(const SNode *sn=G.species.head;sn;sn=sn->next)
                {
                    const Species       &sp = **sn;
                    const size_t * const i  = sp.indx;
                    I[    i[TopLevel] ] = 0;
                    xinj[ i[AuxLevel] ].free();
                }

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
                    if(jail.size>1)
                    {
                        Y_XMLOG(xml, " (*) [sorting #" << jail.size << "]" );
                        //----------------------------------------------------------
                        // find smallest |dC|^2
                        //----------------------------------------------------------
                        MergeSort::Call(jail,BrokenCompare);
                        for(const BrokenNode *node=jail.head;node;node=node->next)
                        {
                            const Broken &bk = **node;
                            Y_XMLOG(xml, "  |- " << bk);

                        }
                    }

                    Broken &broken = **(jail.head);

                    //----------------------------------------------------------
                    // fix the broken law
                    //----------------------------------------------------------
                    Y_XMLOG(xml, " ($) " << broken);
                    for(const Actor *a=broken.law->head;a;a=a->next)
                    {
                        const Species &sp   = a->sp;                           // species
                        const size_t   itop = sp.indx[TopLevel];               // top index
                        const size_t   iaux = sp.indx[AuxLevel];               // aux index
                        const xreal_t  Cnew = broken.cok[iaux];                // new concentration
                        const xreal_t  Cold = C[itop];                         // old concentration
                        const xreal_t  delC = Cnew - Cold; assert(delC>=zero); // delta

                        C[itop] = Cnew;       // update C
                        xinj[iaux] << delC;   // store dC

                        if(xml.verbose)
                        {
                            G.pad( xml() << "  |_" << sp, sp )
                            << " = " << std::setw(15) << real_t(Cnew)
                            << " = " << std::setw(15) << real_t(Cold)
                            << " + " << std::setw(15) << real_t(delC)
                            << std::endl;
                        }
                    }

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
                        probe.bad = probe.law.required(probe.cok,C,xadd);
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

                    
                }

                if(injected)
                {
                    Y_XMLOG(xml, " (*) [modified]");
                    for(const SNode *sn=G.species.head;sn;sn=sn->next)
                    {
                        const Species       &sp = **sn;
                        const size_t * const i  = sp.indx;
                        I[ i[TopLevel] ] = xinj[ i[AuxLevel] ].sum();
                    }

                    if(xml.verbose)
                    {
                        xml() << "C={" << std::endl;
                        G.show(*xml, "\t[", G.species, "]", C, TopLevel);
                        xml() << "}" << std::endl;
                        xml() << "Injected={" << std::endl;
                        G.show(std::cerr, "\t[", G.species, "]", I, TopLevel);
                        xml() << "}" << std::endl;
                    }

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

                for(const GNode *gn =G.head;gn;gn=gn->next)
                    process(C,I,**gn,xml);

            }

        }


    }

}


