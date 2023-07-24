#include "y/data/small/list-node.hpp"
#include "y/data/small/repo-node.hpp"
#include "y/utest/run.hpp"

#include "y/data/list.hpp"

#include "y/object.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace Small
    {
        template <typename T, template <typename> class NODE>
        class ZeroCache
        {
        public:
            Y_ARGS_EXPOSE(T,Type);
            typedef NODE<T> NodeType;

            explicit ZeroCache() noexcept {}
            virtual ~ZeroCache() noexcept {}



            inline NodeType *acquire(ConstType &args)
            {
                NodeType *node = Object::zacquire<NodeType>();
                try {
                    return new (node) NodeType(args);
                }
                catch(...)
                {
                    Object::zrelease(node);
                    throw;
                }
            }

            inline void release(NodeType *node) noexcept
            {
                Object::zrelease(Destructed(node));
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ZeroCache);
        };



        //void Solo;
        //void Coop;

    }

}

Y_UTEST(data_small)
{

    Small::ZeroCache<int,Small::ListNode> ilzc;
    Small::ZeroCache<int,Small::RepoNode> irzc;

    ListOf< Small::ListNode<int> > il;
    ListOf< Small::RepoNode<int> > ir;



}
Y_UDONE()


