#include "y/chemical/reactive/cluster/constellation.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        size_t ClusterConstellation:: maxSimultaneous() const noexcept
        {
            const size_t res = Max( hasOnlyProd.size + hasOnlyReac.size, controllers.size );
            assert(res>0);
            return res;
        }

        static inline
        SignType CompareControllers(const Controller * const lhs,
                                    const Controller * const rhs) noexcept
        {
            return Sign::Of(lhs->primary.indx[TopLevel],rhs->primary.indx[TopLevel]);
        }

        ClusterConstellation:: ClusterConstellation(Equilibria        &eqs,
                                                    const Fragment    &fragment,
                                                    const Constants   &topK,
                                                    XMLog             &xml) :
        ClusterCombinatorics(eqs,fragment,topK,xml),
        hasOnlyReac(),
        hasOnlyProd(),
        controllers(),
        cooperative()
        {

            static const char here[] = "Chemical::Cluster::Constellation";
            Y_XML_SECTION_OPT(xml, here, " eqs='" << size << "'");

            //------------------------------------------------------------------
            //
            // loop over all equilibria
            //
            //------------------------------------------------------------------
            for(const ENode *node=head;node;node=node->next)
            {
                const Equilibrium &eq = **node; assert(eq->size()>0); assert(eq.reac.size+eq.prod.size==eq->size());

                if(xml.verbose)
                {
                   // eqfmt.pad(xml() << "" << eq, eq) << " : "; eqfmt.print(*xml, eq) << " | ";
                    eqfmt.print(xml(), eq) << " | ";
                }

                //--------------------------------------------------------------
                //
                // look for prod-only
                //
                //--------------------------------------------------------------
                if(eq.reac.size<=0)
                {
                    assert(eq.prod.size>0);
                    Coerce(hasOnlyProd) << eq;
                    if(xml.verbose) *xml << "<only prod>" << std::endl;
                    continue;
                }

                //--------------------------------------------------------------
                //
                // look for reac-only
                //
                //--------------------------------------------------------------
                if(eq.prod.size<=0)
                {
                    assert(eq.reac.size>0);
                    Coerce(hasOnlyReac) << eq;
                    if(xml.verbose) *xml << "<only reac>" << std::endl;
                    continue;
                }

                assert(eq.reac.size>0);
                assert(eq.prod.size>0);

                //--------------------------------------------------------------
                //
                // create controller from conserved only species
                //
                //--------------------------------------------------------------
                AutoPtr<Controller> cntl = new Controller(eq,conserved);
                if(cntl->components.reac.size>0 && cntl->components.prod.size>0)
                {
                    for(const Controller *node=controllers.head;node;node=node->next)
                    {
                        if(node->isEquivalentTo(*cntl))
                        {
                            if(xml.verbose)
                            {
                                *xml << "<analogous> @ " << node->primary << std::endl;
                            }
                            goto CONTINUE;
                        }
                    }
                    if(xml.verbose)
                        eqfmt.print(*xml << "<regulates> : ",cntl->components) << std::endl;
                    Coerce(controllers).pushTail( cntl.yield() );


                }
                else
                {
                    if(xml.verbose) *xml << "<redundant>" << std::endl;
                }

            CONTINUE: continue;

            }

            const size_t n = controllers.size;
            if(n<=0) return;;
            //------------------------------------------------------------------
            //
            // re-labelling equilibria AuxLevel
            //
            //------------------------------------------------------------------
            MergeSort::Call( Coerce(controllers), CompareControllers);
            {
                size_t indx=0;
                for(const Controller *cntl=controllers.head;cntl;cntl=cntl->next)
                {
                    const Equilibrium   &eq = cntl->primary;
                    const size_t * const id = eq.indx;
                    Coerce(id[AuxLevel]) = ++indx;
                    if(xml.verbose) eqfmt.pad( xml() << eq, eq ) << std::setw(4) << id[TopLevel] << " -> " << id[AuxLevel] << std::endl;
                }
            }

            //------------------------------------------------------------------
            //
            // cooperativity
            //
            //------------------------------------------------------------------
            Y_XML_SECTION(xml,"Cooperativity");
            {
                BMatrix &coop = Coerce(cooperative);
                coop.make(n,n);
                for(const Controller *lhs=controllers.head;lhs;lhs=lhs->next)
                {
                    const size_t i = **lhs;
                    coop[i][i] = false;
                    for(const Controller *rhs=lhs->next;rhs;rhs=rhs->next)
                    {
                        const size_t j = **rhs;
                        coop[i][j] = coop[j][i] = !(lhs->components.sharesSpeciesWith(rhs->components));
                    }
                    if(xml.verbose)  eqfmt.pad( xml() << lhs->primary, lhs->primary) << " : " << cooperative[i] << std::endl;

                }
            }




        }
        
        ClusterConstellation:: ~ClusterConstellation() noexcept
        {
        }



    }
}
