#include "y/apex/api/ortho/coven/types.hpp"
#include "y/stream/xmlog.hpp"
#include "y/data/list/cxx.hpp"
#include "y/utest/run.hpp"

#include "y/sort/merge.hpp"
#include "y/functor.hpp"

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

                    friend bool operator==(const Content &lhs, const Content &rhs) noexcept
                    {
                        return IList::AreEqual(lhs.my,rhs.my);
                    }

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
                    residue(root.residue.proxy,node)
                    {
                        assert(root.residue.owns(node));
                        assert(1+root.content->size == content->size);
                        assert(root.residue.size-1  == residue.size);
                    }

                    virtual ~Posture() noexcept {}

                    Y_OSTREAM_PROTO(Posture);


                    void xch(Posture &_) noexcept
                    {
                        content.xch(_.content);
                        residue.xch(_.residue);
                    }

                    bool removed(const size_t indx) noexcept
                    {
                        return content.removed(indx) || residue.removed(indx);
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
                                   const size_t   indx,
                                   const FCache & qfcc) :
                    Posture(bank,data.rows,indx),
                    qfcache(qfcc),
                    qfamily( qfcache->query() ),
                    lastIdx( indx ),
                    lastVec( tryIncreaseWith(data[lastIdx] ) ),
                    next(0),
                    prev(0)
                    {

                    }

                    template <typename MATRIX> inline
                    explicit Tribe(const Tribe  &        root,
                                   const MATRIX &        data,
                                   const INode   * const node) :
                    Posture(root,node),
                    qfcache(root.qfcache),
                    qfamily(qfcache->query( *root.qfamily ) ),
                    lastIdx(**node),
                    lastVec( tryIncreaseWith(data[lastIdx] ) ),
                    next(0),
                    prev(0)
                    {
                    }


                    virtual ~Tribe() noexcept
                    {
                        destroy();
                    }

                    Y_OSTREAM_PROTO(Tribe);

                    static SignType Compare(const Tribe * const lhs, const Tribe * const rhs) noexcept
                    {
                        return Natural::Compare(lhs->qfamily->weight(),rhs->qfamily->weight());
                    }

                    template <typename MATRIX> inline
                    void progeny(List         &lineage,
                                 const MATRIX &data) const
                    {

                        for(const INode *node=residue.head;node;node=node->next)
                        {
                            lineage.pushTail( new Tribe(*this,data,node) );
                        }
                        MergeSort::Call(lineage,Compare);
                    }

                    FCache               qfcache;
                    Family *             qfamily;
                    const size_t         lastIdx;
                    const Vector * const lastVec;
                    Tribe *              next;
                    Tribe *              prev;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribe);



                    template <typename READABLE> inline
                    const Vector * tryIncreaseWith(READABLE &a)
                    {
                        try {
                            qfamily->withhold();
                            return qfamily->tryIncreaseWith(a);
                        }
                        catch(...) { destroy(); throw; }
                    }

                    void destroy() noexcept {
                        assert( 0 != qfamily );
                        if( qfamily->liberate() ) qfcache->store(qfamily);
                        qfamily = 0;
                    }
                };


                std::ostream & operator<<(std::ostream &os, const Tribe &tribe)
                {
                    const Posture &posture = tribe;
                    os << posture << "->" << *(tribe.qfamily);
                    return os;
                }

                typedef Functor<void,TL1(const Vector&)> Callback;

                class Tribes : public Tribe::List
                {
                public:
                    static Natural MaxCount(const size_t n)
                    {
                        Natural res = 0;
                        for(size_t k=1;k<=n;++k) res += Natural::Arrange(n,k);
                        return res;
                    }

                    static void Display(const Vector &v)
                    {
                        std::cerr << "\t(+) " << v << std::endl;
                    }

                    template <typename MATRIX> inline
                    explicit Tribes(XMLog &        xml,
                                    Callback     & proc,
                                    const MATRIX & data,
                                    const IBank  & bank,
                                    const FCache & qfcc) :
                    Tribe::List(),
                    vc(qfcc->vcache)
                    {
                        Y_XML_SECTION_OPT(xml, "Coven::Tribes", "[" << data.rows << "][" << data.cols << "]");
                        {
                            const size_t n = data.rows;
                            for(size_t indx=1;indx<=n;++indx)
                                (void) pushTail( new Tribe(data,bank,indx,qfcc) );
                        }
                        initialize(xml,proc);
                    }

                    virtual ~Tribes() noexcept
                    {
                        while(db.size) vc->store( db.popTail() );
                    }

                    Y_OSTREAM_PROTO(Tribes);

                    template <typename MATRIX> inline
                    void generate(const MATRIX &data)
                    {

                        {
                            Tribe::List curr;
                            {
                                Tribe::List chld;
                                for(const Tribe *tribe=head;tribe;tribe=tribe->next)
                                {
                                    tribe->progeny(chld,data);
                                }
                                Tribe::List part;
                                ListOps::Fusion(part,curr,chld,Tribe::Compare);
                                curr.swapWith(part);
                            }
                            swapWith(curr);
                        }
                    }



                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribes);
                    VCache       vc;
                    Vector::List db;

                    const Vector * tryInsert(const Vector &vec)
                    {
                        for(const Vector *v=db.head;v;v=v->next)
                        {
                            switch( Vector::Compare(*v,vec) )
                            {
                                case __Zero__: return 0;
                                case Negative:
                                case Positive:
                                    continue;
                            }
                        }
                        return db.pushTail( vc->query(vec) );
                    }

                    static void RemoveFrom(Tribe::List &tribes, const size_t zid)
                    {
                        for(Tribe *tribe=tribes.head;tribe;tribe=tribe->next)
                        {
                            if(!tribe->removed(zid)) throw Exception("missing index=%u in Tribe", unsigned(zid));
                        }
                    }

                    void noNullVector(XMLog &xml)
                    {
                        Tribe::List ok;
                        while(size>0)
                        {
                            Tribe * const tribe = popHead();
                            if(0==tribe->lastVec)
                            {
                                const size_t zid = tribe->lastIdx;
                                Y_XML_COMMENT(xml, "zero vector #" << zid);
                                delete tribe;
                                RemoveFrom(ok,   zid);
                                RemoveFrom(*this,zid);
                            }
                            else
                            {
                                ok.pushTail(tribe);
                            }
                        }
                        swapWith(ok);
                    }

                    static const Tribe * FoundDuplicateOf(const Vector      &vec,
                                                          const Tribe::List &ok) noexcept
                    {
                        for(const Tribe *guess=ok.head;guess;guess=guess->next)
                        {
                            assert(0!=guess->lastVec);
                            if( vec == *(guess->lastVec) ) return guess;
                        }
                        return 0;
                    }

                    void noDuplicates(XMLog &xml)
                    {
                        Tribe::List ok;
                        while(size>0)
                        {
                            Tribe * const        tribe = popHead();assert(0!=tribe->lastVec);
                            const Tribe *  const guess = FoundDuplicateOf(*(tribe->lastVec),ok);

                            if(0!=guess)
                            {
                                const size_t rid = tribe->lastIdx;
                                Y_XML_COMMENT(xml, "duplicates #" << rid << " and #" << guess->lastIdx);
                                RemoveFrom(*this,rid);
                                for(Tribe *tr=ok.head;tr;tr=tr->next)
                                {
                                    if(!tr->residue.removed(rid)) throw Exception("missing index=%u in Tribe residue", unsigned(rid));
                                }
                                delete tribe;
                            }
                            else
                                ok.pushTail(tribe);
                        }
                        swapWith(ok);
                    }

                    void initialize(XMLog &xml, Callback &proc)
                    {
                        noNullVector(xml);
                        noDuplicates(xml);
                        for(const Tribe *tribe=head;tribe;tribe=tribe->next)
                        {
                            assert(0!=tribe->lastVec);
                            const Vector *vec = tryInsert(*(tribe->lastVec));
                            if(!vec) throw Exception("unexpected multiple initial vector");
                            proc(*vec);
                        }
                    }


                };


                std::ostream & operator<<(std::ostream &os, const Tribes &self)
                {
                    os << '{' << '#' << self.size;
                    if(self.size>0)
                    {
                        os << std::endl;
                        for(const Tribe *tribe=self.head;tribe;tribe=tribe->next)
                        {
                            os << "  " << *tribe << std::endl;
                        }

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
            data[i][j] = ran.in<int>(-5,5);
        }
    }

    //data[1].ld(0);
    //data[3].ld(data[2]);

    Ortho::Coven::IBank  bank;
    Ortho::Metrics       qmtx(cols);
    Ortho::VCache        qvcc( new Ortho::Vector::Cache(qmtx) );
    Ortho::FCache        qfcc( new Ortho::Family::Cache(qvcc) );

    bool   verbose = true;
    XMLog  xml(verbose);

    Ortho::Coven::Callback proc = cfunctor( Ortho::Coven::Tribes::Display );
    Ortho::Coven::Tribes   tribes(xml,proc,data,bank,qfcc);

    Natural count = 0;
    while(tribes.size)
    {
        count += tribes.size;
        std::cerr << tribes << std::endl;
        tribes.generate(data);
    }

    std::cerr << "count=" << count << " / " << Ortho::Coven::Tribes::MaxCount(rows) << std::endl;

}
Y_UDONE()

