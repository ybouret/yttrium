
#include "y/apex/api/ortho/coven/residue.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                Residue:: Residue(const IBank &bank) noexcept :
                IList(bank)
                {
                }

                Residue:: Residue(const Residue &_) :
                IList(_)
                {
                }

                Residue:: Residue(const IBank &bank,
                                 const size_t dims,
                                 const size_t excl) :
                IList(bank)
                {
                    IList &self = *this;
                    for(size_t i=1;i<=dims;++i)
                    {
                        if(i!=excl) self << i;
                    }
                }

                Residue:: Residue(const IBank &       bank,
                                  const INode * const node) :
                IList( bank )
                {
                    assert(0!=node);
                    IList &self = *this;
                    for(const INode *prev=node->prev;prev;prev=prev->prev) self >> **prev;
                    for(const INode *next=node->next;next;next=next->next) self << **next;
                }

                Residue:: ~Residue() noexcept {}

                bool operator==(const Residue &lhs, const Residue &rhs) noexcept
                {
                    return IList::AreEqual(lhs,rhs);
                }

                void  Residue::  xch(Residue &_) noexcept
                {
                    swapWith(_);
                }

                bool  Residue:: removed(const size_t indx) noexcept
                {
                    for(INode *node=head;node;node=node->next)
                    {
                        if(indx==**node) { cutNode(node); return true; }
                    }
                    return false;
                }

            }

        }

    }

}


