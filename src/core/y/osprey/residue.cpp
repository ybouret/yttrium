#include "y/osprey/residue.hpp"

namespace Yttrium
{
    namespace Osprey
    {

        Residue:: Residue(const IBank &bank) noexcept :
        IProxy(bank)
        {

        }

        Residue:: Residue(const IBank &bank,
                          const size_t dims,
                          const size_t excl) :
        IProxy(bank)
        {
            for(size_t i=dims;i>0;--i)
            {
                if(i==excl) continue;
                my >> i;
            }
        }

        Residue:: Residue(const Residue &_) :
        IProxy(_)
        {
        }


        Residue:: Residue(const IBank &       bank,
                          const INode * const node) :
        IProxy(bank)
        {
            assert(0!=node);
            for(const INode *sub=node->prev;sub;sub=sub->prev) my >> **sub;
            for(const INode *sub=node->next;sub;sub=sub->next) my << **sub;
        }

        Residue:: Residue(const IBank &bank,
                          const size_t indx) :
        IProxy(bank)
        {
            my << indx;
        }

        Residue:: ~Residue() noexcept
        {
        }

        void Residue:: removeNull(const size_t zid) noexcept
        {
            INode *node = my.has(zid); assert(0!=node);
            my.cutNode(node);
        }

        void Residue:: exchange(Residue &other) noexcept
        {
            my.swapWith(other.my);
        }

        INode * Residue:: pop() noexcept {
            assert(my.size>0);
            return my.popHead();
        }

        void Residue:: push(INode * const node) noexcept
        {
            assert(0!=node);
            my.pushTail(node);
        }
    }

}
