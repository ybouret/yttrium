#include "y/apex/api/ortho/coven/types.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Apex;

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

                Y_PROXY_IMPL(Content,my)

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
                        for(size_t i=1;i<=dims;++i)
                        {
                            if(i!=excl) (*this) << i;
                        }
                    }

                    explicit Residue(const IBank &       bank,
                                     const INode * const node) :
                    IList(bank)
                    {
                        for(const INode *prev=node->prev;prev;prev=prev->prev) (*this) >> **prev;
                        for(const INode *next=node->next;next;next=next->next) (*this) << **next;
                    }


                    virtual ~Residue() noexcept
                    {
                    }

                    void xch(Residue &_) noexcept
                    {
                        swapWith(_);
                    }

                private:
                    Y_DISABLE_ASSIGN(Residue);
                };


                class Posture
                {
                public:
                    explicit Posture(const IBank &bank,
                                     const size_t dims,
                                     const size_t indx) :
                    content(bank,indx),
                    residue(bank,dims,indx)
                    {
                    }

                    virtual ~Posture() noexcept {}

                    void xch(Posture &_) noexcept
                    {
                        content.xch(_.content);
                        residue.xch(_.residue);
                    }

                    explicit Posture(const Posture &     root,
                                     const INode * const node) :
                    content(root.content,**node),
                    residue(content->proxy,node)
                    {
                        assert(root.residue.owns(node));
                        assert(1+root.content->size == content->size);
                        assert(root.residue.size-1  == residue.size);
                    }

                    Content content;
                    Residue residue;

                private:
                    Y_DISABLE_ASSIGN(Posture);
                };

            }

        }

    }

}

Y_UTEST(apex_coven)
{

}
Y_UDONE()

