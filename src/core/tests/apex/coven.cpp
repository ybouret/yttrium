#include "y/apex/api/ortho/coven/posture.hpp"
#include "y/apex/api/ortho/family.hpp"

#include "y/stream/xmlog.hpp"
#include "y/data/list/cxx.hpp"
#include "y/utest/run.hpp"

#include "y/sort/merge.hpp"
#include "y/functor.hpp"

#include "y/stream/hash/output.hpp"
#include "y/hashing/md.hpp"
#include "y/memory/digest.hpp"
#include "y/hashing/function.hpp"

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

                class Tribe : public Quantized, public Posture
                {
                public:

                    typedef CxxListOf<Tribe> List;
                    static const unsigned OptimizeHyperPlanes = 0x01;
                    static const unsigned UseBasisCompression = 0x02;


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

                    //! add to ordered children
                    template <typename MATRIX> inline
                    void progeny(List          & chld,
                                 const MATRIX  & data,
                                 const unsigned  flag)
                    {

                        assert( chld.isSortedAccordingTo(Tribe::Compare) );
                        {
                            List here;
                            for(const INode *node=residue.head;node;node=node->next)
                            {
                                here.pushTail( new Tribe(*this,data,node) );
                            }
                            MergeSort::Call(here,Compare);
                            ListOps::Fusion(chld,here,Compare);
                        }
                        assert( chld.isSortedAccordingTo(Tribe::Compare) );
                    }

                    void replaceFamilyByFamilyOf(Tribe &better) noexcept
                    {
                        assert(qfamily!=better.qfamily);
                        destroy();
                        (qfamily=better.qfamily)->withhold();
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
                    iteration(1),
                    collected(0),
                    vc(qfcc->vcache),
                    db()
                    {
                        Y_XML_SECTION_OPT(xml, "Coven::Tribes", "initialize [" << data.rows << "][" << data.cols << "]");
                        {
                            const size_t n = data.rows;
                            for(size_t indx=1;indx<=n;++indx)
                                (void) pushTail( new Tribe(data,bank,indx,qfcc) );
                        }
                        initialize(xml,proc);
                        assert(db.size==size);
                        Coerce(collected) = db.size;
                    }

                    virtual ~Tribes() noexcept
                    {
                        while(db.size) vc->store( Coerce(db).popTail() );
                    }

                    Y_OSTREAM_PROTO(Tribes);


                    static inline SignType CompareVectors(const Vector * const lhs,
                                                          const Vector * const rhs) noexcept
                    {
                        return Vector::Compare(*lhs,*rhs);
                    }

                    template <typename MATRIX> inline
                    void generate(XMLog        & xml,
                                  Callback     & proc,
                                  const MATRIX & data,
                                  const unsigned flag)
                    {
                        ++Coerce(iteration);
                        Y_XML_SECTION_OPT(xml, "Coven::Tribes", "iteration=" << iteration << " size=" << size);
                        assert(isSortedAccordingTo(Tribe::Compare));
                        {
                            Tribe::List chld;
                            for(Tribe *tribe=head;tribe;tribe=tribe->next)
                                tribe->progeny(chld,data,flag);
                            swapWith(chld);
                        }
                        assert(isSortedAccordingTo(Tribe::Compare));


                        Y_XML_COMMENT(xml, "#generated   = " << size);
                        collect(proc);
                        Y_XML_COMMENT(xml, "#collected   = " << collected);

                        if(size<=0)
                        {
                            MergeSort::Call( Coerce(db), CompareVectors);
                        }
                        else
                        {
                            if( 0 != (flag & Tribe::UseBasisCompression ) )
                                makeCompression(xml);
                        }

                    }

                    Digest signature(Hashing::Function &H) const
                    {
                        H.set();
                        {
                            HashingStream fp(H);
                            for(const Vector *v=db.head;v;v=v->next) v->serialize(fp);
                        }
                        return Hashing::MD::Of(H);
                    }

                    const size_t iteration;;
                    const size_t collected;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribes);
                    VCache             vc;
                public:
                    const Vector::List db;

                    void makeReplacement(XMLog &xml)
                    {
                        size_t replacement = 0;
                        for(Tribe *curr=head;curr;curr=curr->next)
                        {
                            for(Tribe *prev=curr->prev;prev;prev=prev->prev)
                            {
                                if( (curr->qfamily!=prev->qfamily) && (curr->content==prev->content) )
                                {
                                    curr->replaceFamilyByFamilyOf(*prev);
                                    ++replacement;
                                }
                            }
                        }
                        Y_XML_COMMENT(xml,"#replacement = " << replacement);
                    }

                    void makeCompression(XMLog &xml)
                    {
                        makeReplacement(xml);
                    }

                    void collect(Callback &proc)
                    {
                        size_t & count = Coerce(collected);
                        count = 0;
                        for(const Tribe *tribe=head;tribe;tribe=tribe->next)
                        {
                            const Vector *pVector = tribe->lastVec;      if(0==pVector) continue;
                            const Vector *pSingle = tryInsert(*pVector); if(0==pSingle) continue;
                            proc(*pSingle);
                            ++count;
                        }
                    }

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
                        return Coerce(db).pushTail( vc->query(vec) );
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
                        MergeSort::Call(*this,Tribe::Compare);

                        // collect first
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
#include "y/stream/libc/output.hpp"
#include "y/hashing/sha1.hpp"

namespace
{

    struct Stats
    {
        size_t vc;
        size_t fc;

        inline friend
        std::ostream & operator<<(std::ostream &os, const Stats &s)
        {
            os << "#v = " << s.vc << " | #f = " << s.fc;
            return os;
        }
    };

    template <typename MATRIX> static inline
    Digest Process(XMLog &            xml,
                   const MATRIX &     data,
                   const unsigned     flag,
                   Stats             &stats)
    {

        Ortho::Coven::IBank  bank;
        Ortho::Metrics       qmtx(data.cols);
        Ortho::VCache        qvcc( new Ortho::Vector::Cache(qmtx) );
        Ortho::FCache        qfcc( new Ortho::Family::Cache(qvcc) );

        Ortho::Coven::Callback proc = cfunctor( Ortho::Coven::Tribes::Display );

        Ortho::Coven::Tribes   tribes(xml,proc,data,bank,qfcc);
        OutputFile             fp("coven.dat");

        Natural count = 0;
        while(tribes.size)
        {
            fp("%u %u %u\n", unsigned(tribes.iteration), unsigned(tribes.collected), unsigned(tribes.db.size) );
            count += tribes.size;
            //std::cerr << tribes << std::endl;
            tribes.generate(xml,proc,data,flag);
        }

        std::cerr << "count = " << count << " / " << Ortho::Coven::Tribes::MaxCount(data.rows) << std::endl;

        stats.vc = (*qvcc)->size;
        stats.fc = (*qfcc)->size;


        Hashing::SHA1 H;
        return tribes.signature(H);
    }
}

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
            data[i][j] = ran.in<int>(-1,1);
        }
    }

    //data[1].ld(0);
    //data[3].ld(data[2]);



    bool   verbose = true;
    XMLog  xml(verbose);

    Stats s0; const Digest h0 = Process(xml,data,0,s0);
    Stats s1; const Digest h1 = Process(xml,data,Ortho::Coven::Tribe::UseBasisCompression,s1);
    std::cerr << "h0=" << h0 << " " << s0 << std::endl;
    std::cerr << "h1=" << h1 << " " << s1 << std::endl;
}
Y_UDONE()

