
#include "y/chemical/plexus/cluster.hpp"
#include "y/system/exception.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/woven/subspaces.hpp"
#include "y/kemp/ops/narrow.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/type/temporary.hpp"

#include "y/data/small/heavy/list/bare.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::BareHeavyList<int> Weights;


        static inline
        String MakePositive(const int w, bool &first)
        {
            String ans;
            assert(w>0);
            if(!first)
            {
                ans += '+';
            }

            if(w>1)
            {
                ans += Formatted::Get("%d*",w);
            }

            first = false;
            return ans;
        }

        static inline
        String MakeNegative(const int w, bool &first)
        {
            String ans;
            assert(w<0);
            if(w < -1)
            {
                ans += Formatted::Get("%d*",w);
            }
            else
            {
                ans += '-';
            }
            first = false;
            return ans;
        }



        static inline
        String MakeName(const EList         &eqs,
                        const Readable<int> &cof)
        {
            const size_t n = eqs.size; assert(cof.size()==eqs.size);
            String       ans;
            bool         first=true;
            for(size_t i=1;i<=n;++i)
            {
                const int w = cof[i];
                switch( Sign::Of(w) )
                {
                    case __Zero__: continue;
                    case Positive: ans += MakePositive(w,first); break;
                    case Negative: ans += MakeNegative(w,first); break;
                }
                const Equilibrium &eq = (**eqs.fetch(i));
                assert(eq.indx[SubLevel]==i);
                ans += eq.name;
            }


            return ans;
        }

        void Cluster:: buildCombinatorics(XMLog &xml)
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

            for(const WOVEn::IntegerArray *arr=survey.head;arr;arr=arr->next)
            {
                original.free();
                combined.free();
                vanished.free();

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
                combined.record(stoich);

                for(size_t k=combined.size();k>0;--k)
                {
                    const size_t idx = combined[k];
                    if( !original.search(idx) )
                        throw Specific::Exception("Cluster::Combinatoris","corrupted weights!!");
                }

                vanished |= original;
                vanished ^= combined;

                Y_XMLOG(xml, *arr << " => " << stoich << ":" << original << " \\ " << combined << " -> " << vanished);
                if(vanished.size()<=0)
                    continue;

                std::cerr << "=>" << MakeName(*this,weight) << std::endl;


            }



        }
        
    }

}


