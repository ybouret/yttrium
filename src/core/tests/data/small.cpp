#include "y/data/small/proxy/bare.hpp"
#include "y/data/small/proxy/coop.hpp"

#include "y/data/small/heavy/node.hpp"
#include "y/data/small/light/node.hpp"

#include "y/data/small/proto/list.hpp"
#include "y/data/small/proto/pool.hpp"

#include "y/data/small/light/list.hpp"
#include "y/data/small/heavy/list.hpp"


#include "y/utest/run.hpp"

#include "y/memory/out-of-reach.hpp"


#include "y/string.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace Small
    {
       
        template <typename T>
        class BareHeavyList : public HeavyList<T,BareProxy>
        {
        public:
            typedef HeavyList<T,BareProxy> ListType;
            typedef HeavyNode<T>           NodeType;

            inline explicit BareHeavyList() noexcept : ListType() {}
            inline virtual ~BareHeavyList() noexcept {}
            inline BareHeavyList(const BareHeavyList &_) : ListType(_) {}

        private:
            Y_DISABLE_ASSIGN(BareHeavyList);
        };

    }
}

Y_UTEST(data_small)
{

 

}
Y_UDONE()


