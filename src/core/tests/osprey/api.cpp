#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"

#include "y/data/small/heavy/list/coop.hpp"
#include "y/data/small/light/list/coop.hpp"

#include "y/random/shuffle.hpp"
#include "y/random/park-miller.hpp"

#include "y/data/list/cxx.hpp"
#include "y/functor.hpp"

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
            explicit Posture(const Posture& root,
                            const size_t  ires) :
            content(root.content),
            residue(root.residue)
            {
                content.absorb(residue, ires);
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
                //qfamily->withhold();
            }

            virtual ~Tribe() noexcept
            {
                releaseFamily();
            }

            Y_OSTREAM_PROTO(Tribe);






            template <typename MATRIX>
            void unfold(List& tribes, const MATRIX& data) const
            {
                // create tribe by residue promotion
                std::cerr << "unfolding from " << posture << std::endl;
                size_t             ires = 1;
                for (const INode * node = posture.residue->head; node; node = node->next, ++ires)
                {
                    std::cerr << "\tunfolding with indx=" << **node << " @" << ires << std::endl;
                    tribes.pushTail(new Tribe(data, *this,ires));
                }

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
            os << tribe.posture << *tribe.qfamily;
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

            template <typename MATRIX> inline
            explicit Tribes(const MATRIX& data,
                            const IBank& bank,
                            const QFCache& qfcc) :
            my()
            {
                // initializing
                for (size_t i = 1; i <= data.rows; ++i) {
                    my.pushTail(new Tribe(data, i, bank, qfcc));
                }

                // size: Arrange(n,1) = n

            }

            Y_OSTREAM_PROTO(Tribes);

#if 0
            template <typename MATRIX>
            void generate(const MATRIX& data)
            {
                Tribe::List ng;
                for (const Tribe* tribe = my.head; tribe; tribe = tribe->next)
                {
                    tribe->unfold(ng, data);
                }
                ng.swapWith(my);
            }

            void compress() noexcept
            {
                Tribe::List store;
                while (my.size > 0)
                {
                    Tribe* lhs = my.popHead();
                    for (const Tribe* rhs = store.head; rhs; rhs = rhs->next)
                    {
                        if (ISet::AreEquivalent(lhs->content, rhs->content))
                        {
                            delete lhs;
                            lhs = 0;
                            goto DONE;
                        }
                    }
                DONE:
                    if (0 != lhs)
                        store.pushTail(lhs);
                }
                my.swapWith(store);
            }
#endif


            virtual ~Tribes() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribes);
            Y_PROXY_DECL();

            Tribe::List my;
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




    Matrix<int>      mu(4, 9);
    for (size_t i = 1; i <= mu.rows; ++i)
    {
        for (size_t j = 1; j <= mu.cols; ++j)
        {
            mu[i][j] = ran.in<int>(-5,5);
        }
    }

    Osprey::QMetrics metrics(mu.cols);
    Osprey::QVCache  vcache = new Apex::Ortho::Vector::Cache(metrics);
    Osprey::QFCache  fcache = new Apex::Ortho::Family::Cache(vcache);


    {
        Osprey::Tribes   tribes(mu, bank, fcache);
        std::cerr << "tribes=" << tribes << std::endl;

        for(const Osprey::Tribe *tr=tribes->head;tr;tr=tr->next)
        {
            Osprey::Tribe::List sub;
            tr->unfold(sub,mu);
        }

    }

#if 0
    {
        Osprey::Tribes   tribes(mu, bank, fcache);

        size_t count = 0;
        while (tribes->size > 0)
        {
            count += tribes->size;
            std::cerr << "tribes=" << tribes << std::endl;
            std::cerr << tribes->size << " / " << Natural::Arrange(mu.rows, tribes->head->content->size) << std::endl;
            tribes.generate(mu);
        }
        std::cerr << std::endl;
        std::cerr << "count(" << mu.rows << ")=" << count << " / " << Osprey::Tribes::MaxCount(mu.rows) << std::endl;
    }

    {
        Osprey::Tribes   tribes(mu, bank, fcache);

        size_t count = 0;
        while (tribes->size > 0)
        {
            tribes.compress();
            count += tribes->size;
            std::cerr << "tribes=" << tribes << std::endl;
            std::cerr << tribes->size << " / " << Natural::Arrange(mu.rows, tribes->head->content->size) << std::endl;
            tribes.generate(mu);
        }
        std::cerr << std::endl;
        std::cerr << "count(" << mu.rows << ")=" << count << " / " << Osprey::Tribes::MaxCount(mu.rows) << std::endl;
    }





    for (size_t rows = 1; rows <= 10; ++rows)
    {
        std::cerr << "MaxCount(" << std::setw(2) << rows << ")=" << std::setw(20) << Osprey::Tribes::MaxCount(rows) << std::endl;
    }

#endif

    Y_SIZEOF(Osprey::QFamily);
}
Y_UDONE()
