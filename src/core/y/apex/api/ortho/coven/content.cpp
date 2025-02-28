#include "y/apex/api/ortho/coven/content.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {

            namespace Coven
            {

                Content:: Content(const IBank &bank) noexcept :
                Proxy<const IList>(),
                my(bank)
                {
                }

                Content:: Content(const Content &_) :
                Proxy<const IList>(),
                my(_.my)
                {
                }

                Content:: Content(const IBank &bank,
                                  const size_t indx) :
                Proxy<const IList>(),
                my(bank)
                {
                    (*this) << indx;
                }

                Content:: Content(const Content &root,
                                  const size_t   indx) :
                Proxy<const IList>(),
                my(root.my)
                {
                    (*this) << indx;
                }


                Y_PROXY_IMPL(Content,my)

                bool operator==(const Content &lhs, const Content &rhs) noexcept
                {
                    return IList::AreEqual(lhs.my,rhs.my);
                }

                bool operator!=(const Content &lhs, const Content &rhs) noexcept
                {
                    return !IList::AreEqual(lhs.my,rhs.my);
                }

                Content:: ~Content() noexcept
                {
                }

                Content & Content:: push(INode * const node) noexcept
                {
                    assert(0!=node);
                    ListOps::InsertOrdered(my,node,Compare);
                    return *this;
                }

                Content & Content:: operator<<(const size_t indx)
                {
                    return push(my.proxy->produce(indx));
                }


                void Content:: xch(Content &_) noexcept
                {
                    my.swapWith(_.my);
                }

                bool Content:: removed(const size_t indx) noexcept
                {
                    for(INode *node=my.head;node;node=node->next)
                    {
                        if(indx==**node) { my.cutNode(node); return true; }
                    }
                    return false;
                }

                SignType Content:: Compare(const INode * const lhs,
                                           const INode * const rhs) noexcept
                {
                    return Sign::Of(**lhs,**rhs);
                }

            }
        }


    }

}

