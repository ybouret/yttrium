
#include "y/chemical/reactive/equilibrium/mixed.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        MixedEquilibrium:: ~MixedEquilibrium() noexcept
        {
        }

        MixedEquilibrium :: MixedEquilibrium(const String          &id,
                                             const size_t           topLevel,
                                             const Readable<int>   &weight,
                                             const EqSubSet        &eqset,
                                             const Constants       &topK) :
        Equilibrium(id, topLevel),
        in(),
        nu(),
        K_(topK),
        one(1),
        xmul()
        {
            const size_t n = weight.size();
            for(size_t i=1;i<=n;++i)
            {
                const int cf = weight[i]; if(!cf) continue;
                Coerce(in) << eqset[i].indx[TopLevel];
                Coerce(nu) << cf;
                //std::cerr << "+(" << cf << ")*#" << *in.tail << std::endl;
            }
            assert(in.size>=2);
            assert(nu.size==in.size);
        }

        xreal_t MixedEquilibrium:: getK(real_t)
        {
            xmul.free();
            const IList::NodeType *i = in.head;
            const WList::NodeType *w = nu.head;
            const XVector         &Kv = *K_;
            for(size_t j=in.size;j>0;--j,i=i->next,w=w->next)
            {
                const xreal_t k = Kv[ **i ];
                const int     n = **w;        assert(0!=n);
                if(n>0) xmul.insert(k,n); else xmul.insert(one/k,-n);
            }
            return xmul.product();
        }
    }
}
