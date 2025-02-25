#include "y/apex/api/ortho/coven/types.hpp"
#include "y/data/list/cxx.hpp"
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
                        std::cerr << "sub-content" << std::endl;
                        (*this) << indx;
                        std::cerr << "/sub-content" << std::endl;

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
                        IList &self = *this;
                        for(size_t i=1;i<=dims;++i)
                        {
                            if(i!=excl) self << i;
                        }
                    }

                    explicit Residue(const IBank &       bank,
                                     const INode * const node) :
                    IList(bank)
                    {
                        assert(0!=node);
                        IList &self = *this;
                        std::cerr << "sub-residue" << std::endl;
                        for(const INode *prev=node->prev;prev;prev=prev->prev) self >> **prev;
                        for(const INode *next=node->next;next;next=next->next) self << **next;
                        std::cerr << "/sub-residue" << std::endl;
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

                    explicit Posture(const Posture &     root,
                                     const INode * const node) :
                    content(root.content,**node),
                    residue(content->proxy,node)
                    {
                        std::cerr << "sub-posture" << std::endl;
                        assert(root.residue.owns(node));
                        assert(1+root.content->size == content->size);
                        assert(root.residue.size-1  == residue.size);
                        std::cerr << "/sub-posture" << std::endl;
                    }

                    virtual ~Posture() noexcept {}

                    Y_OSTREAM_PROTO(Posture);


                    void xch(Posture &_) noexcept
                    {
                        content.xch(_.content);
                        residue.xch(_.residue);
                    }


                    Content content;
                    Residue residue;

                private:
                    Y_DISABLE_ASSIGN(Posture);
                };

                std::ostream & operator<<(std::ostream &os, const Posture &self)
                {
                    os << self.content << ':' << self.residue;
                    return os;
                }


                class Tribe : public Quantized, public Posture
                {
                public:

                    typedef CxxListOf<Tribe> List;

                    template <typename MATRIX> inline
                    explicit Tribe(const MATRIX & data,
                                   const IBank  & bank,
                                   const size_t   indx) :
                    Posture(bank,data.rows,indx),
                    next(0),
                    prev(0)
                    {
                        // check dimension
                    }

                    template <typename MATRIX> inline
                    explicit Tribe(const MATRIX &        data ,
                                   const INode   * const node) :
                    Posture(*this,node),
                    next(0),
                    prev(0)
                    {
                    }

                    virtual ~Tribe() noexcept
                    {

                    }

                    template <typename MATRIX> inline
                    void progeny(List         &chld,
                                 const MATRIX &data) const
                    {


                        for(const INode *node=residue.head;node;node=node->next)
                        {
                            assert(0!=node);
                            std::cerr << "chld with node=" << **node << std::endl;
                            chld.pushTail( new Tribe(data,node) );
                            break;
                        }

                    }


                    Tribe * next;
                    Tribe * prev;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribe);
                };


                class Tribes : public Tribe::List
                {
                public:
                    static Natural MaxCount(const size_t n)
                    {
                        Natural res = 0;
                        for(size_t k=1;k<=n;++k) res += Natural::Arrange(n,k);
                        return res;
                    }

                    template <typename MATRIX> inline
                    explicit Tribes(const MATRIX & data,
                                    const IBank  & bank) :
                    Tribe::List()
                    {
                        const size_t n = data.rows;
                        for(size_t i=1;i<=n;++i)
                        {
                            Tribe * const tribe = pushTail( new Tribe(data,bank,i) );
                            (void) tribe;
                        }
                    }

                    virtual ~Tribes() noexcept
                    {
                    }

                    Y_OSTREAM_PROTO(Tribes);

                    template <typename MATRIX> inline
                    void generate(const MATRIX &data)
                    {


                        {
                            Tribe::List chld;
                            for(const Tribe *tribe=head;tribe;tribe=tribe->next)
                            {
                                tribe->progeny(chld,data);
                            }
                            swapWith(chld);
                        }

                    }



                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribes);
                };


                std::ostream & operator<<(std::ostream &os, const Tribes &self)
                {
                    os << "  " << '{' << '#' << self.size;
                    if(self.size>0)
                    {
                        os << std::endl;
                        for(const Tribe *tribe=self.head;tribe;tribe=tribe->next)
                        {
                            os << "    " << *tribe << std::endl;
                        }
                        os << "  ";
                    }
                    os << '}';
                    return os;
                }
            }

        }

    }

}

#include "y/text/ascii/convert.hpp"
#include "y/container/matrix.hpp"
#include "y/random/park-miller.hpp"

Y_UTEST(apex_coven)
{

    Random::ParkMiller ran;
    Y_SIZEOF(Ortho::Coven::Tribe);

    size_t rows = 4; if(argc>1) rows = ASCII::Convert::To<size_t>(argv[1],"rows");
    size_t cols = 5; if(argc>2) cols = ASCII::Convert::To<size_t>(argv[2],"cols");

    Matrix<int> data(rows,cols);
    for(size_t i=1;i<=rows;++i)
    {
        for(size_t j=1;j<=cols;++j)
        {
            data[i][j] = 0;
        }
    }


    Ortho::Coven::IBank  bank;

    {
        Ortho::Coven::Content root(bank,2); std::cerr << root << std::endl;
        Ortho::Coven::Content chld(root,1); std::cerr << chld << std::endl;
        std::cerr << std::endl;
    }

    {
        Ortho::Coven::Residue root(bank,3,2);        std::cerr << root << std::endl;
        {
            Ortho::Coven::Residue chld(bank,root.head);  std::cerr << chld << std::endl;
        }
        {
            Ortho::Coven::Residue chld(bank,root.tail);  std::cerr << chld << std::endl;
        }
    }

    return 0;

    Ortho::Coven::Tribes tribes(data,bank);

    Natural count = 0;
    while(tribes.size)
    {
        std::cerr << tribes << std::endl;
        tribes.generate(data);
    }

}
Y_UDONE()

