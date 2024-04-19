
#include "y/chemical/reactive/cluster.hpp"
#include "y/woven/subspaces.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/apex/mylar.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Coefficients
        {
        public:

            explicit Coefficients(const Readable<int> &w,
                                  const Readable<int> &s,
                                  const size_t         d) :
            weight(CopyOf,w),
            stoich(CopyOf,s),
            snorm1(0),
            order(d)
            {

            }

            virtual ~Coefficients() noexcept {}

            const CxxArray<int> weight;
            const CxxArray<int> stoich;
            const apz           snorm1;
            const size_t        order;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Coefficients);
        };

        void Cluster:: buildCombinatorics(Equilibria &eqs, XMLog &xml)
        {
            WOVEn::IntegerSurvey survey(xml);

            // WOVen survey linear combination
            {
                Matrix<int> NuT(TransposeOf,Nu);
                Matrix<int> NuTx;
                Apex::Mylar::Compress(NuTx,NuT);
                Y_XMLOG(xml,"NuT  = "   << NuT);
                Y_XMLOG(xml,"NuTx = "  << NuTx);
                WOVEn::Explore(NuTx, survey, false, xml);
                survey.sort();
            }

            // convert to stoichiometry
            const size_t  n = Nu.rows;
            const size_t  m = Nu.cols;
            CxxArray<int> weight(n,0);
            CxxArray<apz> apzSto(m,0);

            for(const WOVEn::IntegerArray *arr=survey.head;arr;arr=arr->next)
            {
                assert(arr->size()==n);
                const Readable<const apz> &w = *arr;
                for(size_t i=n;i>0;--i) weight[i] = w[i].cast<int>("weight");
                for(size_t j=m;j>0;--j)
                {
                    apzSto[j] = 0;
                    for(size_t i=n;i>0;--i)
                        apzSto[j] += w[j] * Nu[i][j];
                }
            }

        }
    }

}
