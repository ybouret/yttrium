
#include "y/chemical/plexus/clusters.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/system/exception.hpp"

#include "y/apex/api/ortho/coven/survey/integer.hpp"
#include "y/apex/api/count-non-zero.hpp"

#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;
        using namespace Apex;
        using namespace Ortho;
        using namespace Coven;

        class MixTab : public Quantized, public CxxArray<int,Memory::Dyadic>
        {
        public:
            typedef CxxArray<int,Memory::Dyadic> ArrayType;

            explicit MixTab(const Readable<apz> &arr,
                            const iMatrix       &topo) :
            ArrayType(arr.size()),
            stoi(topo.cols)
            {
                const size_t n = arr.size();
                for(size_t i=n;i>0;--i)
                {
                    if( !arr[i].tryCast( (*this)[i]) )
                        throw Specific::Exception(Cluster::CallSign,"coefficient overflow");
                }

                const size_t m = stoi.size();
                for(size_t j=m;j>0;--j)
                {
                    apz sum = 0;
                    for(size_t i=n;i>0;--i)
                        sum += arr[i] * topo[i][j];
                    if( !sum.tryCast(stoi[j]))
                        throw Specific::Exception(Cluster::CallSign,"stoichiometry overflow");
                }

            }

            virtual ~MixTab() noexcept
            {
            }


            bool isEfficientFor(const ClusterType &cl) const
            {
                AddressBook target;
                for(const SNode *sn=cl.species->head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    if( 0 != sp(stoi,SubLevel) ) target |= sp;
                }
                


            }

            ArrayType stoi;

            MixTab *next;
            MixTab *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixTab);
        };

        void Cluster:: combinatorics(XMLog &xml)
        {
            Y_XML_SECTION(xml, "combinatorics");
            
            const IntegerSurvey survey(xml,topologyT,0);
            std::cerr << "survey=" << survey << std::endl;
            if(survey->size<=0) {
                Y_XML_COMMENT(xml,"no combinatorics");
                return;
            }

            for(const IntegerSurvey::ArrayType *arr=survey->head;arr;arr=arr->next)
            {
                MixTab mix(*arr,topology);
                std::cerr << mix << " => " << mix.stoi << std::endl;
                if(mix.isEfficientFor(**this))
                {

                }
            }

            std::cerr << "sizeof(MixTab)=" << sizeof(MixTab) << std::endl;
        }

    }

}
