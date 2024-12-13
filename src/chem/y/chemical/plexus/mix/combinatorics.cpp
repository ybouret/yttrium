
#include "y/chemical/plexus/mix.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/woven/subspaces.hpp"
#include "y/system/exception.hpp"
#include "y/system/exception.hpp"
#include "y/chemical/reactive/equilibrium/hybrid.hpp"
#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef WOVEn::ArrayOf<int>  iArray;
        typedef WOVEn::ArraysOf<int> iArrays;


        //static inline int MakeOpposite(const int i) noexcept { return -i; }

        static inline
        iArrays * computeWeights(XMLog &            xml,
                                 const Matrix<int> &topology)
        {
            WOVEn::IntegerSurvey survey(xml);
            const Matrix<int>    mu(TransposeOf,topology);
            WOVEn::Explore(mu,survey,false);
            survey.sort();
            return  new iArrays(survey,"combination weight",2);
        }


        void Mix:: buildCombinatorics(XMLog &xml, Equilibria &eqs)
        {
            Y_XML_SECTION(xml, "Mix::Combinatorics");

            //------------------------------------------------------------------
            //
            //
            // compute all weights and stoichiometries
            //
            //
            //------------------------------------------------------------------
            const AutoPtr<iArrays> weight = computeWeights(xml,my.topology);
            const AutoPtr<iArrays> stoich = new iArrays(my.topology,*weight);

            assert(weight->size==stoich->size);

            ELists &mine = Coerce(order);
            {
                ELists temp(my.size);
                mine.swapWith(temp);
            }

            EList &primary = mine[1];
            for(const ENode *en=my.head;en;en=en->next)
            {
                primary << **en;
            }

            for(const iArray *W = weight->head, *S=stoich->head; W; W=W->next,S=S->next)
            {
                const Readable<int> &w = *W;
                const Readable<int> &s = *S;

                //--------------------------------------------------------------
                //
                // load original species
                //
                //--------------------------------------------------------------
                AddressBook original;
                for(const ENode *en=primary.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if( 0 == eq(w,SubLevel) ) continue;
                    eq.addSpeciesTo(original);
                }

                //--------------------------------------------------------------
                //
                // find combined species
                //
                //--------------------------------------------------------------
                AddressBook combined;
                for(const SNode *sn=my.species.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    if(sp(*S,SubLevel)!=0) combined += sp;
                }
                assert(combined.size()>0);


                if( !original.contains(combined) ) throw Specific::Exception(CallSign,"corrupted WOVEn stoichiometry");
                if(  original.size() == combined.size() ) continue; // inefficient
                assert(combined.size()<original.size());


                //--------------------------------------------------------------
                //
                // create hybrid in equilibria
                //
                //--------------------------------------------------------------
                const String       name = HybridEquilibrium::MakeName(primary,w);
                const size_t       indx = eqs->size() + 1;
                const Equilibrium &eq   = eqs.add( new HybridEquilibrium(name,indx,primary,w,my.species,s,eqs.K) );

                //--------------------------------------------------------------
                //
                // record hybrid as replica
                //
                //--------------------------------------------------------------
                my.replica(eq);
                eqs.updateFragment();
                mine[W->order] << eq;
            }

            
            Coerce(grade) = new Grade(my,*genus);
        }


    }

}
