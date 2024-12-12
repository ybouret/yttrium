
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

            const AutoPtr<iArrays> weight = computeWeights(xml,my.topology);
            const AutoPtr<iArrays> stoich = new iArrays(my.topology,*weight);

            assert(weight->size==stoich->size);


            for(const iArray *W = weight->head, *S=stoich->head; W; W=W->next,S=S->next)
            {
                const Readable<int> &w = *W;
                const Readable<int> &s = *S;
                // load original species
                AddressBook original;
                for(const ENode *en=my.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if( 0 == eq(w,SubLevel) ) continue;
                    eq.addSpeciesTo(original);
                }

                // find combined species
                AddressBook combined;
                for(const SNode *sn=my.species.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    if(sp(*S,SubLevel)!=0) combined += sp;
                }
                assert(combined.size()>0);

                // check
                SList originalList; original.sendTo(originalList);
                SList combinedList; combined.sendTo(combinedList);

                if( ! originalList.contains(combinedList) )
                    throw Specific::Exception(CallSign,"corrupted WOVEn stoichiometry");

                if( combinedList.contains(originalList) )
                {
                    assert( combinedList.alike(originalList) );
                    continue; // inefficient
                }

                assert(combinedList.size>0);
                assert(combinedList.size<originalList.size);



                // create hybrid
                const String name = HybridEquilibrium::MakeName(my,w);
                const size_t indx = eqs->size() + 1;

                AutoPtr<Equilibrium> eq = new HybridEquilibrium(name,indx,my,w,my.species,s,eqs.K);
                eq->print(std::cerr) << std::endl;

            }



        }
    }

}
