
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





        void Cluster:: buildCombinatorics(XMLog &xml, Equilibria &primary)
        {
            Y_XML_SECTION(xml, "Combinatorics");

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
                WOVEn::Explore(mat,survey,false);
            }
            survey.sort();
            //------------------------------------------------------------------
            //
            // Each survey is a combination of reactions
            //
            //------------------------------------------------------------------
            const size_t   n = size;
            const size_t   m = species.size;
            CxxArray<int>  stoich(m);
            CxxArray<int>  weight(n);
            WOVEn::Indices original(m);
            WOVEn::Indices combined(m);
            WOVEn::Indices vanished(m);

            const ENode *last = tail; assert(0!=last);
            for(const WOVEn::IntegerArray *arr=survey.head;arr;arr=arr->next)
            {
                original.free();
                combined.free();
                vanished.free();

                //--------------------------------------------------------------
                // compute stoichiometry and collect original species
                //--------------------------------------------------------------
                stoich.ld(0);
                // fetch weights
                for(size_t i=n;i>0;--i)
                {
                    const int w = weight[i] = (*arr)[i].cast<int>("weight");
                    if(0==w) continue;
                    const Readable<int> &nu = Nu[i];
                    original.record(nu);
                    for(size_t j=m;j>0;--j)
                        stoich[j] += w * nu[j];
                }

                //--------------------------------------------------------------
                // check combined species
                //--------------------------------------------------------------
                combined.record(stoich);

                for(size_t k=combined.size();k>0;--k)
                {
                    const size_t idx = combined[k];
                    if( !original.search(idx) )
                        throw Specific::Exception("Cluster::Combinatoris","corrupted weights!!");
                }

                //--------------------------------------------------------------
                // check vanishing
                //--------------------------------------------------------------
                vanished |= original;
                vanished ^= combined;

                //Y_XMLOG(xml, *arr << " => " << stoich << ":" << original << " \\ " << combined << " -> " << vanished);
                if(vanished.size()<=0)
                    continue;

                //--------------------------------------------------------------
                // create mixed equilibrium
                //--------------------------------------------------------------
                AutoPtr<MixedEquilibrium> mx = new MixedEquilibrium(primary->size()+1,*this,weight);
                (*this) << *mx;
                try {
                    // insert mx
                    Equilibrium &eq = primary.append(mx.yield());

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
                    primary.update(eq);
                    update(eq);

                    // update sub-level
                    Coerce(eq.indx[SubLevel]) = size;
                }
                catch(...)
                {
                    cutTail();
                    throw;
                }



            }

            if(xml.verbose)
            {
                for(last=last->next;last;last=last->next)
                {
                    display(xml(), **last) << std::endl;
                }
            }

        }

    }

}


