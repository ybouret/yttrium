//! \file

#ifndef Y_Apex_Ortho_Coven_Content_Included
#define Y_Apex_Ortho_Coven_Content_Included 1

#include "y/apex/api/ortho/coven/types.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                class Content : public Proxy<const IList>
                {
                public:
                    explicit Content(const IBank &bank) noexcept :
                    Proxy<const IList>(),
                    my(bank)
                    {
                    }
                    
                    explicit Content(const Content &_) :
                    Proxy<const IList>(),
                    my(_.my)
                    {
                    }
                    
                    
                    //! with first index
                    explicit Content(const IBank &bank,
                                     const size_t indx) :
                    Proxy<const IList>(),
                    my(bank)
                    {
                        (*this) << indx;
                    }
                    
                    explicit Content(const Content &root,
                                     const size_t   indx) :
                    Proxy<const IList>(),
                    my(root.my)
                    {
                        (*this) << indx;
                        
                    }
                    
                    virtual ~Content() noexcept
                    {
                    }
                    
                    Content & push(INode * const node) noexcept
                    {
                        assert(0!=node);
                        ListOps::InsertOrdered(my,node,Compare);
                        return *this;
                    }
                    
                    Content & operator<<(const size_t indx)
                    {
                        return push(my.proxy->produce(indx));
                    }
                    
                    void xch(Content &_) noexcept
                    {
                        my.swapWith(_.my);
                    }
                    
                    friend bool operator==(const Content &lhs, const Content &rhs) noexcept;

                    bool removed(const size_t indx) noexcept
                    {
                        for(INode *node=my.head;node;node=node->next)
                        {
                            if(indx==**node) { my.cutNode(node); return true; }
                        }
                        return false;
                    }
                    
                private:
                    Y_DISABLE_ASSIGN(Content);
                    Y_PROXY_DECL();
                    IList my;
                    
                    static SignType Compare(const INode * const lhs,
                                            const INode * const rhs) noexcept
                    {
                        return Sign::Of(**lhs,**rhs);
                    }
                };
            }
            
        }
        
    }
}

#endif
