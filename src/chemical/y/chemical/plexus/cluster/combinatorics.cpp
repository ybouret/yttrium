
#include "y/chemical/plexus/cluster.hpp"
#include "y/system/exception.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/woven/subspaces.hpp"
#include "y/kemp/ops/narrow.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/type/temporary.hpp"
#include "y/chemical/reactive/equilibrium/mixed.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        bool Cluster:: hasConserved(const Actors &actors) const noexcept
        {
            for(const Actor *a=actors->head;a;a=a->next)
            {
                if(conserved.book.has(a->sp)) return true;
            }
            return false;
        }



        void Cluster:: buildCombinatorics(XMLog      &xml,
                                          Equilibria &eqs,
                                          XWritable  &shK)
        {
            Y_XML_SECTION(xml, "Combinatorics");

            //------------------------------------------------------------------
            //
            // Prepare Order and register primary eqs
            //
            //------------------------------------------------------------------
            { Lists _(size); Coerce(order).swapWith(_); }

            Lists &ord    = Coerce(order);
            for(const ENode *en=head;en;en=en->next)
            {
                ord[1] << **en;
            }


            //------------------------------------------------------------------
            //
            // survey of orthogonal to NuT
            //
            //------------------------------------------------------------------
            WOVEn::IntegerSurvey survey(xml);
            {
                const Matrix<int> NuT(TransposeOf,Nu);
                Matrix<int>       mat;

                Kemp::Narrow::Down(mat,NuT);
                Y_XMLOG(xml, "NuT = " << NuT);
                Y_XMLOG(xml, "mat = " << mat);;
                if(MKL::Rank::Of(mat)!=Nu.rows)
                    throw Specific::Exception("Cluster::Combinatorics","corrupted compressed rank");

                const Temporary<bool> quiet(Coerce(xml.verbose),false);
                Y_XMLOG(xml,"exploring combinatorics...");
                WOVEn::Explore(mat,survey,false);
            }
            survey.sort();

            //------------------------------------------------------------------
            //
            // Each survey is a combination of equilibria
            //
            //------------------------------------------------------------------
            const size_t   n = size;
            const size_t   m = species.size;
            CxxArray<int>  stoich(m);
            CxxArray<int>  weight(n);
            WOVEn::Indices original(m);
            WOVEn::Indices combined(m);
            WOVEn::Indices vanished(m);

            //------------------------------------------------------------------
            //
            // build mixed equilibria
            //
            //------------------------------------------------------------------
            for(const WOVEn::IntegerArray *arr=survey.head;arr;arr=arr->next)
            {
                original.free();
                combined.free();
                vanished.free();

                //--------------------------------------------------------------
                // compute stoichiometry and collect original species
                //--------------------------------------------------------------
                stoich.ld(0);
                size_t nc = 0;
                for(size_t i=n;i>0;--i)
                {
                    const int w = weight[i] = (*arr)[i].cast<int>("weight");
                    if(0==w) continue;
                    const Readable<int> &nu = Nu[i];
                    original.record(nu);
                    for(size_t j=m;j>0;--j) 
                        stoich[j] += w * nu[j];
                    ++nc;
                }

                //--------------------------------------------------------------
                // retrieve and check combined species
                //--------------------------------------------------------------
                combined.record(stoich);

                for(size_t k=combined.size();k>0;--k)
                {
                    const size_t idx = combined[k];
                    if( !original.search(idx) )
                        throw Specific::Exception("Cluster::Combinatorics","corrupted weights!!");
                }

                //--------------------------------------------------------------
                // check vanishing species
                //--------------------------------------------------------------
                vanished |= original;
                vanished ^= combined;

                if(vanished.size()<=0)
                    continue;

                //--------------------------------------------------------------
                // create mixed equilibrium
                //--------------------------------------------------------------
                AutoPtr<MixedEquilibrium> mx = new MixedEquilibrium(eqs->size()+1,*this,weight,shK);

                for(const ENode *en=head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if(mx->analogous(eq))
                    {
                        std::cerr << "[ANALOGOUS] " << eq.name << " and " << mx->name << std::endl;
                        exit(1);
                    }
                }

                (*this) << *mx;
                try {

                    Equilibrium &eq = eqs.append(mx.yield());

                    // fill equilibirium
                    for(const SNode *sn=species.head;sn;sn=sn->next)
                    {
                        const Species &s  = **sn;
                        const size_t   j  = s.indx[SubLevel];
                        const int      nu = stoich[j];
                        if(0==nu) continue;
                        eq(nu,s);
                    }

                    // update formatting
                    eqs.update(eq);
                    update(eq);

                    // update sub-level
                    Coerce(eq.indx[SubLevel]) = size;

                    ord[nc] << eq;
                }
                catch(...)
                {
                    cutTail();
                    throw;
                }

            }

            //------------------------------------------------------------------
            //
            // Post-process all equilibria
            //
            //------------------------------------------------------------------
            if(xml.verbose)
            {
                for(size_t i=1;i<=ord.size();++i)
                {
                    const EList &l = ord[i];
                    if(l.size<=0) continue;
                    Y_XML_SECTION_OPT(xml, "Order", " n='" << i << "'");
                    for(const ENode *en=ord[i].head;en;en=en->next)
                    {
                        display(xml(), **en) << std::endl;
                    }
                }
            }

            {
                Y_XML_SECTION(xml, "Classification");
                for(const ENode *en=head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    switch(eq.kind)
                    {
                        case Nebulous: throw Specific::Exception(eq.name.c_str(), "empty equilibrium in cluster!!");
                        case ReacOnly: Coerce(roaming.reacOnly) << eq; continue;
                        case ProdOnly: Coerce(roaming.prodOnly) << eq; continue;
                        case Standard: break;
                    }
                    assert(Standard==eq.kind);
                    if( hasConserved(eq.reac) || hasConserved(eq.prod) )
                    {
                        Coerce(limited) << eq;
                    }
                    else
                    {
                        Coerce(roaming.standard) << eq;
                    }
                }

                Y_XML_LIST(xml,roaming.prodOnly);
                Y_XML_LIST(xml,roaming.reacOnly);
                Y_XML_LIST(xml,roaming.standard);
                Y_XML_LIST(xml,limited);
            }

            //------------------------------------------------------------------
            //
            // Make global topology
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml, "Topology");
                Matrix<int> &itopo = Coerce(iTopo);
                XMatrix     &xtopo = Coerce(xTopo);
                xtopo.make(size,species.size);
                itopo.make(size,species.size);
                for(const ENode *en=head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const size_t       ei = eq.indx[SubLevel];
                    XWritable         &xnu = xtopo[ei];
                    Writable<int>     &inu = itopo[ei];
                    eq.topology(xnu,SubLevel);
                    eq.topology(inu,SubLevel);
                    if(xml.verbose)
                    {
                        uuid.pad( xml() << eq.name, eq) << " : " << inu << std::endl;
                    }
                }
            }

            //------------------------------------------------------------------
            //
            // Make global attached
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml, "Attached");
                Matrix<bool> &flag = Coerce(attached);
                flag.make(size,size);
                flag.ld(false);
                for(const ENode *lhs=head;lhs;lhs=lhs->next)
                {
                    const Equilibrium &lEq = **lhs;
                    const size_t       lid = lEq.indx[SubLevel];
                    Writable<bool>    &bit = flag[lid];
                    bit[lid] = true;
                    for(const ENode *rhs=lhs->next;rhs;rhs=rhs->next)
                    {
                        const Equilibrium &rEq = **rhs;
                        const size_t       rid = rEq.indx[SubLevel];
                        bit[rid] = flag[rid][lid] = lEq.linkedTo(rEq);
                    }
                    if(xml.verbose)
                    {
                        uuid.pad( xml() << lEq.name, lEq) << " : " << bit << std::endl;
                    }
                }
            }


            //------------------------------------------------------------------
            //
            // Make global detached
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml, "Detached");
                Matrix<bool> &flag = Coerce(detached);
                flag.make(size,size);
                flag.ld(false);
                for(const ENode *lhs=head;lhs;lhs=lhs->next)
                {
                    const Equilibrium &lEq = **lhs;
                    const size_t       lid = lEq.indx[SubLevel];
                    Writable<bool>    &bit = flag[lid];
                    //bit[lid] = false;
                    for(const ENode *rhs=lhs;rhs;rhs=rhs->next)
                    {
                        const Equilibrium &rEq = **rhs;
                        const size_t       rid = rEq.indx[SubLevel];
                        bit[rid] = flag[rid][lid] = !lEq.linkedToConserved(rEq,conserved.book);
                    }
                    if(xml.verbose) uuid.pad( xml() << lEq.name, lEq) << " : " << bit << std::endl;
                }
            }



            postBuildConservations(xml);



            //------------------------------------------------------------------
            //
            // Summary
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml, "Summary");
                Y_XMLOG(xml, "#primary equilibria: " << Nu.rows);
                Y_XMLOG(xml, "#derived equilibria: " << size-Nu.rows);
                Y_XMLOG(xml, "#total   equilibria: " << size << " (*)");
            }
        }

#if 1
        namespace
        {
            class SLists
            {
            public:
                virtual ~SLists() noexcept {}


                explicit SLists(const Components  &components,
                                const AddressBook &conserved) :
                reac(), prod()
                {
                    reac.free();
                    prod.free();
                    components.separate(reac, prod, conserved);
                }

                bool alikeForward(const SLists &rhs) const noexcept
                {
                    return reac.alike(rhs.reac) && prod.alike(rhs.prod);
                }

                bool alikeReverse(const SLists &rhs) const noexcept
                {
                    return reac.alike(rhs.prod) && prod.alike(rhs.reac);
                }

                bool areEquivalentTo(const SLists &rhs) const noexcept
                {
                    return alikeForward(rhs) || alikeReverse(rhs);
                }


                SList reac;
                SList prod;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SLists);
            };
        }

        void Cluster:: postBuildConservations(XMLog &xml)
        {
            //------------------------------------------------------------------
            //
            // Linking limited equilibria to laws
            //
            //------------------------------------------------------------------
            if( laws.isValid() )
            {
                Y_XML_SECTION(xml, "ConservationLink");
                const AddressBook &auth = unbounded.book; // authorized extra species
                const AddressBook &cons = conserved.book; // conserved species database
                for(const Conservation::Laws::Group *G=laws->groups.head;G;G=G->next)
                {
                    const SList &crew = G->crew; // authorized primary species
                    for(const Conservation::LNode *ln=G->head;ln;ln=ln->next)
                    {
                        const CLaw &law = **ln;
                        Y_XML_SECTION(xml,law.name);
                        for(const ENode *en=limited.head;en;en=en->next)
                        {
                            // pre-selecting equilibria
                            const Equilibrium &lhs = **en;
                            bool               ok  = true;
                            for(Components::ConstIterator it=lhs->begin();it!=lhs->end();++it)
                            {
                                const Species &sp = (**it).sp;
                                if(auth.has(sp))   continue; // unbounded, that's ok
                                if(crew.has(sp))   continue; // in crew, that's ok
                                ok = false; break;           // limited but not in crew => drop
                            }
                            if(!ok) continue;

                            // post-selecting equilibria
                            ok = true;
                            const SLists lc(lhs,cons);

                            for(const ENode *tmp=law.base.head;tmp;tmp=tmp->next)
                            {
                                const Equilibrium &rhs = **tmp;
                                const SLists rc(rhs,cons);

                                if( rc.areEquivalentTo(lc) )
                                {
                                    ok = false;
                                    break;
                                }
                            }

                            if(!ok) continue;

                            Coerce(law.base) << lhs;
                            Y_XMLOG(xml, "(+) " << lhs);
                        }
                    }
                }
            }
        }
#endif
        
    }

}


