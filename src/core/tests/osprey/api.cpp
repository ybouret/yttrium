#include "y/apex/api/ortho/family.hpp"
#include "y/apex/api/count-non-zero.hpp"

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

#include "y/container/matrix.hpp"
#include "y/system/exception.hpp"

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
            IProxy(const IProxy &_) :
            Proxy<const IList>(),
            my(_.my)
            {}

        protected:
            explicit IProxy(const IBank &bank) noexcept :
            Proxy<const IList>(),
            my(bank)
            {}
            
            IList my;

        private:
            Y_DISABLE_ASSIGN(IProxy);
            Y_PROXY_DECL();
            friend class Content;
        };

        Y_PROXY_IMPL(IProxy,my)


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

            void removeNull(const size_t zid) noexcept
            {
                INode *node = my.has(zid); assert(0!=node);
                my.cutNode(node);
            }

            virtual ~Residue() noexcept
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Residue);
        };


        class Content : public IProxy
        {
        public:
            explicit Content(const IBank &bank,
                             const size_t indx)   :
            IProxy(bank)
            {
                my << indx;
            }

            Content(const Content &root,
                    const size_t   indx) :
            IProxy(root)
            {
                *this << indx;
                assert(root->size+1==my.size);
            }

            Content & operator<<(const size_t indx) {
                assert( !my.has(indx) );
                ListOps::InsertOrdered(my,my.proxy->produce(indx), Compare);
                return *this;
            }

            //! steal residue and renumber
            Content & operator<<(Residue &residue)
            {
                while( residue->size > 0)
                {
                    assert( !my.has( ** (residue->head)) );
                    my.pushTail( residue.my.popHead() );
                }
                const size_t dims = my.size;
                INode *      node = my.head;
                for(size_t i=1;i<=dims;++i,node=node->next)
                {
                    assert(0!=node);
                    **node = i;
                }
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


        class Posture
        {
        public:
            explicit Posture(const IBank &bank,
                             const size_t dims,
                             const size_t excl) :
            content(bank,excl),
            residue(bank,dims,excl),
            quantum(dims)
            {
                assert(content->size+residue->size==quantum);
            }

            explicit Posture(const Posture &     root,
                             const INode * const node) :
            content(root.content,**node),
            residue(root.residue->proxy,node),
            quantum(root.quantum)
            {
                assert(root.residue->owns(node));
                assert(root.residue->size-1==residue->size);
                assert(root.content->size+1==content->size);
                assert(content->size+residue->size==quantum);
            }




            virtual ~Posture() noexcept
            {

            }

            static bool AreEqual(const Posture &lhs, const Posture &rhs) noexcept
            {
                return IList::AreEqual(*lhs.content,*rhs.content) && IList::AreEqual(*lhs.residue,*rhs.residue);
            }

            void flush() noexcept
            {
                assert(content->size+residue->size==quantum);
                content << residue;
                assert(content->size+residue->size==quantum);
            }

            friend std::ostream & operator<<(std::ostream &os, const Posture &self)
            {
                os << self.content << ':' << self.residue;
                return os;
            }

            Content      content;
            Residue      residue;
            const size_t quantum;

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
            typedef CxxListOf<Tribe> List;

            template <typename MATRIX> inline
            explicit Tribe(const MATRIX  & data,
                           const IBank   & bank,
                           const QFCache & qfcc,
                           const size_t    indx) :
            posture(bank,data.rows,indx),
            qfcache(qfcc),
            qfamily(qfcache->query()),
            lastIdx(indx),
            lastVec(addWith(data[lastIdx])),
            next(0),
            prev(0)
            {

            }

            // building from node from root's residue
            template <typename MATRIX> inline
            explicit Tribe(const MATRIX  &     data,
                           const Tribe   &     root,
                           const INode * const node) :
            posture(root.posture,node),
            qfcache(root.qfcache),
            qfamily(qfcache->query(*root.qfamily)),
            lastIdx(**node),
            lastVec(addWith(data[lastIdx])),
            next(0),
            prev(0)
            {
                if(0==lastVec)
                {
                    std::cerr << "root=" << root << std::endl;
                    std::cerr << std::endl << " Unfolding Dependent Vector!!" << std::endl;
                    exit(0);
                }
            }

            virtual ~Tribe() noexcept { destroy(); }

            friend std::ostream & operator<<(std::ostream &os, const Tribe &tribe)
            {
                os << tribe.posture << " -> " << *tribe.qfamily;
                return os;
            }

            template <typename MATRIX> inline
            void unfold(Tribe::List &tribes, const MATRIX &data)
            {
                assert(0!=qfamily);


                if(posture.residue->size<=0) return;

                const Ortho::Quality q = qfamily->quality;
                std::cerr << "Unfolding " << QMetrics::QualityText(q) << " Family" << std::endl;
                switch(q)
                {
                    case Ortho::Degenerate: throw Specific::Exception("Tribe::Unfold","%s family!",QMetrics::QualityText(q));
                    case Ortho::Generating: return; // nothing to add
                    case Ortho::Hyperplane:
                    case Ortho::Fragmental:
                        break;
                }

                // generic processing
                for(const INode *node=posture.residue->head;node;node=node->next)
                {
                    tribes.pushTail( new Tribe(data,*this,node) );
                }
            }


            Posture        posture;
            QFCache        qfcache;
            QFamily       *qfamily;
            const size_t   lastIdx;
            const QVector *lastVec;
            Tribe         *next;
            Tribe         *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribe);

            template <typename T>
            const  QVector * addWith(const Readable<T> &a)
            {
                try
                {
                    const QVector *ans = qfamily->tryIncreaseWith(a);
                    qfamily->withhold();
                    return ans;
                }
                catch(...)
                {
                    destroy();
                    throw;
                }
            }

            void destroy() noexcept
            {
                assert(0!=qfamily);
                if(qfamily->liberate())
                    qfcache->store(qfamily);
                qfamily = 0;
            }

        };

        typedef Functor<void,TL1(const QVector &)> Callback;


        class Tribes : public Proxy<const Tribe::List>
        {
        public:
            static Apex::Natural MaxCount(const size_t n)
            {
                Apex::Natural res=0;
                for(size_t i=1;i<=n;++i) res += Apex::Natural::Arrange(n,i);
                return res;
            }

            static void Display(const QVector &v)
            {
                std::cerr << "\t(+) " << v << std::endl;
            }


            template <typename MATRIX> inline
            explicit Tribes(Callback      & proc,
                            const MATRIX  & data,
                            const IBank   & bank,
                            const QFCache & qfcc) :
            my(),
            vc(qfcc->vcache),
            db()
            {
                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                {
                    const size_t rows = data.rows;
                    for(size_t indx=1;indx<=rows;++indx)
                    {
                        (void) my.pushTail( new Tribe(data,bank,qfcc,indx) )->lastVec;
                    }
                }

                //--------------------------------------------------------------
                // take care of null vector
                //--------------------------------------------------------------
                {
                    Tribe::List ok;
                    while(my.size>0)
                    {
                        Tribe *tr = my.popHead();
                        if(0==tr->lastVec)
                        {
                            const size_t zid = tr->lastIdx;
                            No(zid,ok);
                            No(zid,my);
                            delete tr;
                            continue;
                        }
                        ok.pushTail(tr);
                    }
                    my.swapWith(ok);
                }

                //--------------------------------------------------------------
                // first processing
                //--------------------------------------------------------------
                for(Tribe *tr=my.head;tr;tr=tr->next)
                {
                    assert(0!=tr->lastVec);
                    assert(0!=tr->qfamily);
                    assert((*(tr->qfamily))->size>0);
                    assert(tr->qfamily->quality != Ortho::Degenerate);
                    const QVector *vec = tryInsert(*(tr->lastVec));
                    if(0!=vec)
                        proc(*vec);
                }
            }

            virtual ~Tribes() noexcept
            { while(db.size>0) vc->store( Coerce(db).popTail() ); }


            friend std::ostream & operator<<(std::ostream &os, const Tribes &self)
            {
                if(self.my.size<=0) os << "{}";
                else
                {
                    os << "{ #" << self.my.size << std::endl;
                    for(const Tribe *tr=self->head;tr;tr=tr->next)
                    {
                        os << *tr << std::endl;
                    }
                    os << "}";

                }
                return os;
            }


            template <typename MATRIX> inline
            void generate(Callback &     proc,
                          const MATRIX & data)
            {
                // create new generation
                {
                    Tribe::List ng;
                    for(Tribe *tr=my.head;tr;tr=tr->next)
                    {
                        tr->unfold(ng,data);
                    }
                    my.swapWith(ng);
                }

                // check something happened
                for(Tribe *tr=my.head;tr;tr=tr->next)
                {
                    const QVector * const qv = tr->lastVec;

                }


            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribes);
            Y_PROXY_DECL();
            Tribe::List   my;
            QVCache       vc;
        public:
            const QVector::List db;

        private:

            //! remove zid from residue of tribes
            static void No(const size_t zid, Tribe::List &tribes) noexcept
            {
                for(Tribe *tr=tribes.head;tr;tr=tr->next)
                {
                    tr->posture.residue.removeNull(zid);
                }
            }

            //! try insert vector in database
            const QVector *tryInsert(const QVector &rhs)
            {
                for(const QVector *lhs=db.head;lhs;lhs=lhs->next)
                {
                    if( *lhs == rhs ) return 0;
                }
                return Coerce(db).pushTail( vc->query(rhs) );
            }
        };

        Y_PROXY_IMPL(Tribes,my)
    }

}

Y_UTEST(osprey)
{
    Random::ParkMiller ran;

    size_t  rows = 5; if(argc>1) rows = ASCII::Convert::To<size_t>(argv[1],"rows");
    size_t  dims = 6; if(argc>2) dims = ASCII::Convert::To<size_t>(argv[2],"dims");


    Matrix<int>      data(rows,dims);
    for (size_t i = 1; i <= data.rows; ++i)
    {
        for (size_t j = 1; j <= data.cols; ++j)
        {
            data[i][j] = ran.in<int>(-5,5);
        }
    }

    //data[2].ld(0);
    data[4].ld(0);
    std::cerr << "data=" << data << std::endl;

    Osprey::IBank    bank;
    Osprey::QMetrics metrics(data.cols);
    Osprey::QVCache  vcache = new Apex::Ortho::Vector::Cache(metrics);
    Osprey::QFCache  fcache = new Apex::Ortho::Family::Cache(vcache);
    void (*proc_)(const Osprey::QVector &) = Osprey::Tribes::Display;
    Osprey::Callback proc(proc_);
    Osprey::Tribes   tribes(proc,data,bank,fcache);

    size_t count = 0;
    while(tribes->size)
    {
        count += tribes->size;
        std::cerr << tribes << std::endl;
        tribes.generate(proc,data);
    }
    std::cerr << "count=" << count << "/" << Osprey::Tribes::MaxCount(data.rows) << std::endl;



    for(size_t n=1;n<=16;++n)
    {
        const Apex::Natural nmax = Osprey::Tribes::MaxCount(n);
        std::cerr << n << " -> " << nmax.toDec(3) << std::endl;
    }


}
Y_UDONE()



