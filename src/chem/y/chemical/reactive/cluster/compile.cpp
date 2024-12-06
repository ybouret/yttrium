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

            {
                AddressBook &rebels = Coerce(unbounded.book);
                for(const SNode *sn=eqs.species.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    if(conserved.book.has(sp)) continue;
                    rebels += sp;
                }
            }
            Coerce(unbounded).compile();
            Y_XMLOG(xml,"unbounded=" << unbounded.list);

        }

        void Cluster:: compile(XMLog &xml)
        {
            const EList &el = eqs;
            const SList &sl = eqs.species;
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
            // create combinatorics
            //
            //------------------------------------------------------------------
            WOVEn::IntegerSurvey survey(xml);
            {
                const Matrix<int>    mu(TransposeOf,eqs.iTopology);
                WOVEn::Explore(mu,survey,false);
            }

            survey.sort();
            for(const WOVEn::IntegerArray *warr=survey.head;warr;warr=warr->next)
            {
                const WOVEn::IntegerArray &comb = *warr;
                std::cerr << "testing " << comb << std::endl;
            }





        }
    }

}

