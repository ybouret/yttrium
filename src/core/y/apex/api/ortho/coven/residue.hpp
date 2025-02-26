
//! \file

#ifndef Y_Apex_Ortho_Coven_Residue_Included
#define Y_Apex_Ortho_Coven_Residue_Included 1

#include "y/apex/api/ortho/coven/types.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                class Residue : public IList
                {
                public:
                    explicit Residue(const IBank &bank) noexcept :
                    IList(bank)
                    {
                    }

                    explicit Residue(const Residue &_) :
                    IList(_)
                    {
                    }

                    explicit Residue(const IBank &bank,
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

                    static inline const IBank & check(const IBank &bank)
                    {
                        assert(bank.isValid());
                        return bank;
                    }

                    explicit Residue(const IBank &       bank,
                                     const INode * const node) :
                    IList( check(bank) )
                    {
                        assert(0!=node);
                        IList &self = *this;
                        for(const INode *prev=node->prev;prev;prev=prev->prev) self >> **prev;
                        for(const INode *next=node->next;next;next=next->next) self << **next;
                    }


                    virtual ~Residue() noexcept
                    {
                    }

                    void xch(Residue &_) noexcept
                    {
                        swapWith(_);
                    }

                    bool removed(const size_t indx) noexcept
                    {
                        for(INode *node=head;node;node=node->next)
                        {
                            if(indx==**node) { cutNode(node); return true; }
                        }
                        return false;
                    }

                private:
                    Y_DISABLE_ASSIGN(Residue);
                };

            }

        }

    }

}

#endif

