#include "y/chemical/reactive/cluster.hpp"
#include "y/system/exception.hpp"

#include "y/woven/survey/integer.hpp"
#include "y/woven/subspaces.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: classifySpecies(XMLog &xml)
        {

            Y_XML_SECTION(xml,"SpeciesClassification");

            // extracting conserved from laws
            {
                AddressBook &abiding = Coerce(conserved.book);
                for(const Conservation::Law *ln=laws->head;ln;ln=ln->next)
                {
                    for(const Actor *a = (*ln)->head;a;a=a->next)
                        abiding |= a->sp;
                }
            }
            Coerce(conserved).compile();
            Y_XMLOG(xml,"conserved=" << conserved.list);

            // excluding undounded from conserved
            {
                AddressBook &rebels = Coerce(unbounded.book);
                for(const SNode *sn=my.species.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    if(conserved.book.has(sp)) continue;
                    rebels += sp;
                }
            }
            Coerce(unbounded).compile();
            Y_XMLOG(xml,"unbounded=" << unbounded.list);
        }

        void Cluster:: compile(Equilibria &eqs, XMLog &xml)
        {
            const EList &el = my;
            const SList &sl = my.species;
            Y_XML_SECTION_OPT(xml,Grouping::CallSign, el << '/' << sl);
            assert(laws.isEmpty());

            //------------------------------------------------------------------
            //
            //
            // create laws
            //
            //
            //------------------------------------------------------------------
            Coerce(laws) = new Conservation::Laws(*this,xml);

            //------------------------------------------------------------------
            //
            //
            // classify species accordingly
            //
            //
            //------------------------------------------------------------------
            classifySpecies(xml);

            //------------------------------------------------------------------
            //
            //
            // create combinatorics
            //
            //
            //------------------------------------------------------------------
            WOVEn::IntegerSurvey survey(xml);
            {
                Y_XML_SECTION(xml, "WOVEn::Explore");
                const Matrix<int>    mu(TransposeOf,my.iTopology);
                WOVEn::Explore(mu,survey,false);
                survey.sort();
            }

            const size_t            nspc = sl.size;
            const apz               zero = 0;
            Vector<apz,MemoryModel> stoi(nspc,0);
            AddressBook             original;
            AddressBook             combined;
            for(const WOVEn::IntegerArray *warr=survey.head;warr;warr=warr->next)
            {
                const WOVEn::IntegerArray &comb = *warr;


                // initialize state
                stoi.ld(zero);
                original.free();
                combined.free();

                for(const ENode *en=my.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const Indexed     &id = eq;
                    const apz         &cf = id(comb,SubLevel);

                    if(zero==cf)
                        continue;

                    // use equilibrium
                    eq.addSpeciesTo(original);
                    for(Equilibrium::ConstIterator it=eq->begin();it!=eq->end();++it)
                    {
                        const Component &cm = *it;
                        const Species   &sp = cm.actor.sp;
                        const apz        nu = cm.actor.nu; assert(nu>0);
                        switch(cm.role)
                        {
                            case Product:  sp(stoi,SubLevel) += nu * cf; break;
                            case Reactant: sp(stoi,SubLevel) -= nu * cf; break;
                        }
                    }
                }


                for(const SNode *sn=sl.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    if(zero!=sp(stoi,SubLevel)) combined |= sp;
                }

                SList oldSpecies; original.sendTo(oldSpecies);
                SList newSpecies; combined.sendTo(newSpecies);
                SList evaporated(oldSpecies);

                DBOps::Revamp<SList>::Sort(oldSpecies);
                DBOps::Revamp<SList>::Sort(newSpecies);


                switch( Sign::Of(newSpecies.size,oldSpecies.size) )
                {
                    case __Zero__:
                        assert( oldSpecies.alike(newSpecies) );
                        Y_XMLOG(xml, comb << " : " << oldSpecies << " => " << newSpecies << " / useless");
                        continue;

                    case Positive:
                        Y_XMLOG(xml, comb << " : " << oldSpecies << " => " << newSpecies << " / corrupted!");
                        throw Specific::Exception(Grouping::CallSign, "corrupted combinatorics!");

                    case Negative:
                        evaporated.subtract(newSpecies);
                        Y_XMLOG(xml, comb << " : " << oldSpecies << " => " << newSpecies << " / (-) " << evaporated);
                        break;
                }

                



            }





        }
    }

}

