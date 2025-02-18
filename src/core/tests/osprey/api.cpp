#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"

#include "y/data/small/heavy/list/coop.hpp"
#include "y/data/small/light/list/coop.hpp"

#include "y/random/shuffle.hpp"
#include "y/random/park-miller.hpp"

#include "y/data/list/cxx.hpp"
#include "y/functor.hpp"

#include "y/text/ascii/convert.hpp"
#include "y/stream/hash/output.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/md.hpp"
#include "y/memory/digest.hpp"

using namespace Yttrium;
using namespace Apex;

namespace Yttrium
{
    namespace Osprey
    {
        typedef Small::CoopHeavyList<size_t> IList; //!< raw list
        typedef IList::NodeType              INode; //!< alias
        typedef IList::ProxyType             IBank; //!< alias

        class IProxy : public Proxy<const IList>
        {
        public:
            virtual ~IProxy() noexcept {}
            IProxy(const IProxy &_) : my(_.my) {}

        protected:
            explicit IProxy(const IBank &bank) noexcept : my(bank){}
            IList my;

        private:
            Y_DISABLE_ASSIGN(IProxy);
            Y_PROXY_DECL();
        };

        Y_PROXY_IMPL(IProxy,my)

        class Content : public IProxy
        {
        public:
            explicit Content(const IBank &bank,
                             const size_t indx)   :
            IProxy(bank)
            {
                my << indx;
            }

            Content(const Content &_,
                    const size_t   indx) :
            IProxy(_)
            {
                *this << indx;
            }

            Content & operator<<(const size_t indx) {
                assert( !my.has(indx) );
                ListOps::InsertOrdered(my,my.proxy->produce(indx), Compare);
                return *this;
            }

            virtual ~Content() noexcept
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Content);
            static SignType Compare(const INode *lhs, const INode *rhs) noexcept
            {
                return Sign::Of(**lhs,**rhs);
            }
        };

        class Residue : public IProxy
        {
        public:

            explicit Residue(const IBank &bank,
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

            explicit Residue(const IBank &       bank,
                             const INode * const node) :
            IProxy(bank)
            {
                for(const INode *sub=node->prev;sub;sub=sub->prev) my >> **sub;
                for(const INode *sub=node->next;sub;sub=sub->next) my << **sub;
            }

            virtual ~Residue() noexcept
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Residue);
        };

        class Posture
        {
        public:
            explicit Posture(const IBank &bank,
                             const size_t dims,
                             const size_t excl) :
            content(bank,excl),
            residue(bank,dims,excl)
            {
            }

            explicit Posture(const Posture &     root,
                             const INode * const node) :
            content(root.content,**node),
            residue(root.residue->proxy,node)
            {
            }



            virtual ~Posture() noexcept
            {

            }

            friend std::ostream & operator<<(std::ostream &os, const Posture &self)
            {
                os << self.content << ':' << self.residue;
                return os;
            }

            Content content;
            Residue residue;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Posture);
        };


        typedef Apex::Ortho::Metrics QMetrics;
        typedef Apex::Ortho::Vector  QVector;
        typedef Apex::Ortho::Family  QFamily;
        typedef Apex::Ortho::VCache  QVCache;
        typedef Apex::Ortho::FCache  QFCache;


        class Tribe
        {
        public:

            template <typename MATRIX> inline
            explicit Tribe(const MATRIX  & data,
                           const IBank   & bank,
                           const QFCache & qfcc,
                           const size_t    indx) :
            posture(bank,data.rows,indx),
            qfcache(qfcc),
            qfamily(qfcache->query()),
            lastVec(addQFamilyWith(data[indx]))
            {

            }

#if 0
            template <typename MATRIX> inline
            explicit Tribe(const MATRIX  & data,
                           const Tribe   & root,
                           const INode * const node) :
            {

            }
#endif

            virtual ~Tribe() noexcept { releaseQFamily(); }

            Posture        posture;
            QFCache        qfcache;
            QFamily       *qfamily;
            const QVector *lastVec;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribe);

            template <typename T>
            const  QVector * addQFamilyWith(const Readable<T> &a)
            {
                try
                {
                    return qfamily->tryIncreaseWith(a);
                }
                catch(...)
                {
                    releaseQFamily();
                    throw;
                }
            }
            void releaseQFamily() noexcept
            {
                assert(0!=qfamily);
                if(qfamily->liberate())
                    qfcache->store(qfamily);
                qfamily = 0;
            }

        };


    }

}

Y_UTEST(osprey)
{
    Random::ParkMiller ran;

    Osprey::IBank      bank;

    Osprey::Posture    posture(bank,8,3);
    std::cerr << posture << std::endl;
    {
        Osprey::Posture sub(posture,posture.residue->head);
        std::cerr << sub << std::endl;
    }

}
Y_UDONE()

#if 0

namespace Yttrium
{
    namespace Osprey
    {

        typedef Small::CoopHeavyList<size_t> IList; //!< raw list
        typedef IList::NodeType              INode; //!< alias
        typedef IList::ProxyType             IBank; //!< alias



        class ISet : public Proxy<const IList>
        {
        public:
            explicit ISet(const IBank& ibank) noexcept :
            Proxy<const IList>(),
            my(ibank)
            {
            }

            explicit ISet(const IBank& ibank,
                          const size_t indx) :
            my(ibank)
            {
                my << indx;
            }

            explicit ISet(const IBank& ibank,
                          const size_t  dims,
                          const size_t  excl) :
            my(ibank)
            {
                for (size_t i = 1; i <= dims; ++i)
                {
                    if (i != excl)
                        my << i;
                }
            }

            ISet(const ISet& _) :
            Proxy<const IList>(),
            my(_.my)
            {
            }

            bool search(const size_t i) const noexcept
            {
                for (const INode* node = my.head; node; node = node->next)
                {
                    if (i == **node) return true;
                }
                return false;
            }

            size_t absorbHeadOf(ISet& residue) noexcept
            {
                assert(residue->size > 0);
                return **my.pushTail(residue.my.popHead());
            }

            void absorb(ISet& residue, const size_t ires) noexcept
            {
                my.pushTail(residue.get(ires));
            }



            bool atTail(const size_t i)
            {
                if (search(i)) return false;
                my << i;      return true;
            }

            bool atHead(const size_t i)
            {
                if (search(i)) return false;
                my >> i;      return true;
            }


            void shuffle(Random::Bits& ran) noexcept {
                Random::Shuffle::List(my, ran);
            }

            static bool AreEquivalent(const ISet& lhs, const ISet& rhs) noexcept
            {
                if (lhs->size != rhs->size) return false;
                for (const INode* node = rhs->head; node; node = node->next)
                {
                    if (!lhs.search(**node)) return false;
                }
                return true;
            }

            virtual ~ISet() noexcept
            {
            }

        private:
            Y_DISABLE_ASSIGN(ISet);
            Y_PROXY_DECL();
            INode* get(const size_t ires) noexcept { return my.pop(my.fetch(ires)); }
            IList my;

        };

        Y_PROXY_IMPL(ISet, my)

        class Posture
        {
        public:
            //! initialize content with indx, residue with other
            explicit Posture(const IBank& bank,
                             const size_t dims,
                             const size_t indx) :
            content(bank, indx),
            residue(bank, dims, indx)
            {
                assert(dims == content->size + residue->size);
            }

            // initialize with root and promotion of residue[ires]
            explicit Posture(const Posture & root,
                             const size_t    ires) :
            content(root.content),
            residue(root.residue)
            {
                content.absorb(residue,ires);
            }


            //! cleanup
            virtual ~Posture() noexcept
            {
            }

            friend std::ostream &operator<<(std::ostream &os, const Posture &self)
            {
                os << self.content << ':' << self.residue;
                return os;
            }

            // try to promote next residue, return 0 if none
            size_t promote() noexcept
            {
                return residue->size > 0 ? content.absorbHeadOf(residue) : 0;
            }


            ISet content;
            ISet residue;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Posture);
        };



        typedef Apex::Ortho::Metrics QMetrics;
        typedef Apex::Ortho::Vector  QVector;
        typedef Apex::Ortho::Family  QFamily;
        typedef Apex::Ortho::VCache  QVCache;
        typedef Apex::Ortho::FCache  QFCache;


        typedef Functor<void,TL1(const QVector &)> Callback;


        class Tribe : public Quantized, public Proxy<const QFamily>
        {
        public:
            typedef CxxListOf<Tribe> List;


            template <typename MATRIX>
            explicit Tribe(const MATRIX &  data,
                           const size_t    indx,
                           const IBank  &  bank,
                           const QFCache & qfcc) :
            qfamily(0),
            lastVec(0),
            posture(bank, data.rows, indx),
            qfcache(qfcc),
            next(0),
            prev(0)
            {
                Coerce(qfamily) = qfcache->query();
                tryExpandWith(data[indx]);
            }

            template <typename MATRIX>
            explicit Tribe(const MATRIX &  data,
                           const Tribe  &  root,
                           const size_t    ires) :
            qfamily(0),
            lastVec(0),
            posture(root.posture,ires),
            qfcache(root.qfcache),
            next(0),
            prev(0)
            {
                Coerce(qfamily) = qfcache->query( *root.qfamily );
                tryExpandWith(data[**(posture.content->tail)]);
            }

            virtual ~Tribe() noexcept
            {
                releaseFamily();
            }

            Y_OSTREAM_PROTO(Tribe);


            
            template <typename MATRIX>
            void unfold(List& tribes, const MATRIX& data) const
            {
                size_t             ires = 1;
                for (const INode * node = posture.residue->head; node; node = node->next, ++ires)
                {
                    tribes.pushTail(new Tribe(data, *this,ires));
                }

            }

            bool tryReplaceFamilyBy(const Tribe &other) noexcept
            {
                if( ISet::AreEquivalent(posture.content,other.posture.content) )
                {
                    std::cerr << posture.content << " <=> " << other.posture.content << std::endl;
                    releaseFamily();
                    ( Coerce(qfamily) = other.qfamily)->withhold();
                    return true;
                }
                else
                    return false;
            }

        private:
            QFamily* const       qfamily;
        public:
            const QVector* const lastVec;
            const Posture        posture;
            QFCache              qfcache;

            Tribe* next;
            Tribe* prev;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribe);
            Y_PROXY_DECL();

            void releaseFamily() noexcept
            {
                assert(0 != qfamily);
                if(qfamily->liberate())
                    qfcache->store(qfamily);
                Coerce(qfamily) = 0;
            }

            template <typename ARRAY> inline
            void tryExpandWith(const ARRAY& a)
            {
                try {
                    Coerce(lastVec) = qfamily->tryIncreaseWith(a);
                    qfamily->withhold();
                }
                catch (...)
                {
                    releaseFamily();
                    throw;
                }
            }

        };

        Y_PROXY_IMPL(Tribe, *qfamily)

        std::ostream& operator<<(std::ostream& os, const Tribe& tribe)
        {
            os << tribe.posture << "->" << *tribe.qfamily;
            return os;
        }

        class Tribes : public Proxy<const Tribe::List>
        {
        public:

            static Apex::Natural MaxCount(const size_t rows)
            {
                Apex::Natural res;
                for (size_t i = 1; i <= rows; ++i) res += Apex::Natural::Arrange(rows, i);
                return res;
            }

            static void Display(const QVector &v)
            {
                std::cerr << "\t(+) " << v << std::endl;
            }


            template <typename MATRIX> inline
            explicit Tribes(Callback      &  proc,
                            const MATRIX  &  data,
                            const IBank   &  bank,
                            const QFCache &  qfcc) :
            my(),
            vc(qfcc->vcache)
            {
                // initializing: size = Arrange(n,1) = n
                for (size_t i = 1; i <= data.rows; ++i)
                    my.pushTail(new Tribe(data, i, bank, qfcc));

                // collect initial
                collect(proc);
            }

            Y_OSTREAM_PROTO(Tribes);


            bool alreadyHas(const QVector * const rhs) const noexcept
            {
                assert(0!=rhs);
                for(const QVector *   lhs = db.head; lhs; lhs=lhs->next)
                {
                    switch( QVector::Compare(*lhs,*rhs) )
                    {
                        case __Zero__: return true;
                        case Negative:
                        case Positive:
                            break;
                    }
                }
                return false;
            }

            void collect(Callback &proc)
            {
                for(const Tribe *tribe=my.head;tribe;tribe=tribe->next)
                {
                    const QVector * const rhs = tribe->lastVec;
                    if(0==rhs) continue;
                    if(alreadyHas(rhs)) continue;
                    proc(* Coerce(db).pushTail( vc->query(*rhs) ));
                }
            }

            template <typename MATRIX>
            void generate(Callback     & proc,
                          const MATRIX & data,
                          const bool     doComp)
            {
                Tribe::List ng;
                for (const Tribe* tribe = my.head; tribe; tribe = tribe->next)
                {
                    tribe->unfold(ng, data);
                }
                ng.swapWith(my);
                collect(proc);
                if(doComp)
                {
                    compress();
                }

            }

            //! signature of database
            Digest signature(Hashing::Function &h) const
            {
                h.set();
                HashingStream fp(h);
                size_t        cnt = 0;
                for(const QVector *v=db.head;v;v=v->next)
                {
                    cnt += v->serialize(fp);
                }
                h.run(&cnt,sizeof(cnt));
                return Hashing::MD::Of(h);
            }

            void compress() noexcept
            {
                for(Tribe *curr=my.head;curr;curr=curr->next)
                {
                    for(Tribe *prev=curr->prev;prev;prev=prev->prev)
                    {
                        if(curr->tryReplaceFamilyBy(*prev))
                            goto DONE;
                    }
                    DONE:;
                }
            }



            virtual ~Tribes() noexcept
            {
                while(db.size>0) vc->store(Coerce(db).popTail());
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribes);
            Y_PROXY_DECL();
            Tribe::List   my;
            QVCache       vc;
        public:
            const QVector::List db;
        };

        Y_PROXY_IMPL(Tribes, my)

        std::ostream& operator<<(std::ostream& os, const Tribes& tribes)
        {
            if (tribes->size <= 0) os << "{}";
            else
            {
                os << '{';
                os << " #" << tribes->size;
                os << std::endl;
                for (const Tribe* tribe = tribes->head; tribe; tribe = tribe->next)
                {
                    os << "  " << *tribe << std::endl;
                }
                os << '}' << std::endl;
            }
            return os;
        }

    }
}

using namespace Yttrium;
using namespace Apex;

#include "y/container/matrix.hpp"

static inline String GroupBy(const size_t n, const String &s)
{
    String res;
    size_t count = 0;
    for(size_t i=s.size();i>0;--i)
    {
        res >> s[i];
        if(++count==n) { res >> ' '; count=0; }
    }
    return res;
}


template <typename MATRIX>
static inline
Digest BuildTribes(Hashing::Function & hfcn,
                   Osprey::Callback    proc,
                   const MATRIX    &   data,
                   Osprey::IBank   &   bank,
                   size_t          &   cacheSize,
                   const bool          compress)
{
    Osprey::QMetrics metrics(data.cols);
    Osprey::QVCache  vcache = new Apex::Ortho::Vector::Cache(metrics);
    Osprey::QFCache  fcache = new Apex::Ortho::Family::Cache(vcache);
    {
        Osprey::Tribes   tribes(proc, data, bank, fcache);

        size_t count = 0;
        while (tribes->size > 0)
        {
            count += tribes->size;
            std::cerr << "tribes=" << tribes << std::endl;
            std::cerr << "-------- " << tribes->size << " / " << Natural::Arrange(data.rows, tribes->head->posture.content->size) << std::endl;
            tribes.generate(proc,data,compress);
        }
        std::cerr << std::endl;
        std::cerr << "count(" << data.rows << ")=" << count << " / " << Osprey::Tribes::MaxCount(data.rows) << std::endl;

        cacheSize = (*vcache)->size;
        for(const Osprey::QVector *v=tribes.db.head;v;v=v->next) {
            Osprey::Tribes::Display(*v);
        }
        std::cerr << "collect=" << tribes.db.size << "/cache=" << cacheSize <<  std::endl;
        return tribes.signature(hfcn);
    }
}

Y_UTEST(osprey)
{
    Random::ParkMiller ran;
    Osprey::IBank      bank;


    { Osprey::ISet _(bank); }

    for (size_t dims = 1; dims <= 5; ++dims)
    {
        for (size_t nrot = 0; nrot < dims; ++nrot)
        {
            Osprey::ISet iset(bank, dims, nrot);
            Osprey::ISet copy(iset);
            Osprey::ISet rset(iset); rset.shuffle(ran);
            std::cerr << iset << " / " << copy << " / " << rset << std::endl;
            Y_ASSERT(Osprey::ISet::AreEquivalent(iset, copy));
            Y_ASSERT(Osprey::ISet::AreEquivalent(iset, rset));

        }
    }

    size_t  rows = 4; if(argc>1) rows = ASCII::Convert::To<size_t>(argv[1],"rows");
    size_t  dims = 5; if(argc>2) dims = ASCII::Convert::To<size_t>(argv[2],"dims");



    Matrix<int>      mu(rows,dims);
    for (size_t i = 1; i <= mu.rows; ++i)
    {
        for (size_t j = 1; j <= mu.cols; ++j)
        {
            mu[i][j] = ran.in<int>(-5,5);
        }
    }

    Hashing::SHA1 hfcn;
    void (*proc_)(const Osprey::QVector &) = Osprey::Tribes::Display;
    Osprey::Callback proc(proc_);
    size_t           cRaw = 0;
    const Digest     dRaw = BuildTribes(hfcn,proc,mu,bank,cRaw,false);
    size_t           cCmp = 0;
    const Digest     dCmp = BuildTribes(hfcn,proc,mu,bank,cCmp,true);
    std::cerr << "dRaw=" << dRaw << "/" << cRaw <<std::endl;
    std::cerr << "dCmp=" << dCmp << "/" << cCmp << std::endl;

    for (size_t rows = 1; rows <= 16; ++rows)
    {
        const Apex::Natural nmax = Osprey::Tribes::MaxCount(rows);
        const String        nrep = nmax.toDec();
        const String        ndsp = GroupBy(3,nrep);
        std::cerr << "MaxCount(" << std::setw(2) << rows << ")=" << std::setw(20) << ndsp  << " (" << nmax->bits << " bits)" << std::endl;
    }
    Y_SIZEOF(Osprey::QFamily);
}
Y_UDONE()

#endif

