
#include "y/chemical/plexus/cluster/mixtab.hpp"
#include "y/system/exception.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const MixTab::CallSign = "Mixing";

        MixTab:: MixTab(const IntegerSurvey::ArrayType &arr,
                        const iMatrix                  &topo) :
        iArrayType(arr.size()),
        ncof(arr.ncof),
        stoi(topo.cols),
        next(0),
        prev(0)
        {
            const size_t n = arr.size();
            for(size_t i=n;i>0;--i)
                if( !arr[i].tryCast( (*this)[i]) ) throw Specific::Exception(CallSign,"coefficient overflow");


            const size_t m = stoi.size();
            for(size_t j=m;j>0;--j)
            {
                apz sum = 0;
                for(size_t i=n;i>0;--i)
                    sum += arr[i] * topo[i][j];
                if( !sum.tryCast(stoi[j])) throw Specific::Exception(CallSign,"stoichiometry overflow");
            }

        }


        MixTab:: ~MixTab() noexcept {}

        SignType MixTab:: Compare(const MixTab * const lhs,
                                  const MixTab * const rhs) noexcept
        {
            switch( Sign::Of(lhs->ncof,rhs->ncof) )
            {
                case Negative: return Negative;
                case Positive: return Positive;
                case __Zero__:
                    break;
            }
            return LexicographicCompare(*lhs,*rhs);
        }


        bool  MixTab:: isEfficientFor(const ClusterTopology &cl, AddressBook &source) const
        {
            static const char msg[] = "corrupted stoichiometry ";
            source.free();

            // gather created species from stoichiometry
            AddressBook target;
            for(const SNode *sn=cl->species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if( 0 != sp(stoi,SubLevel) ) target += sp;
            }

            // gather original species from intial equilibria
            {
                size_t n=cl.N;
                for(const ENode *en=cl->equilibria->head;n-- > 0;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if( 0 != eq(*this,SubLevel) ) eq.gatherSpeciesIn(source);
                }
            }


            // compare
            const size_t src = source.size();
            const size_t tgt = target.size();

            switch( Sign::Of(tgt,src) )
            {
                case Positive:
                    throw Specific::Exception(CallSign,"%sexcess",msg);

                case __Zero__:
                    if( !(source==target) )
                        throw Specific::Exception(CallSign,"%sequality!!",msg);
                    return false;

                case Negative:
                    break;
            }

            for(AddressBook::Iterator it=target.begin();it!=target.end();++it)
            {
                const void * addr = *it;
                if(!source.remove_(addr))
                    throw Specific::Exception(CallSign,"%scompression",msg);
            }

            return true;
        }


    }

}


