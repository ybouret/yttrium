
#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"

#include "y/data/small/heavy/list/coop.hpp"
#include "y/data/small/light/list/coop.hpp"

#include "y/random/shuffle.hpp"
#include "y/random/park-miller.hpp"

#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Osprey
    {

        typedef Small::CoopHeavyList<size_t> IndexList; //!< raw list
        typedef IndexList::NodeType          IndexNode;
        typedef IndexList::ProxyType         IndexBank;



        class ISet : public Proxy<const IndexList>
        {
        public:
            explicit ISet(const IndexBank &ibank) noexcept :
            Proxy<const IndexList>(),
            my(ibank)
            {
            }

            explicit ISet(const IndexBank &ibank,
                          const size_t     indx) :
            my(ibank)
            {
                my << indx;
            }

            explicit ISet(const IndexBank &ibank,
                          const size_t     dims,
                          const size_t     excl) :
            my(ibank)
            {
                for(size_t i=1;i<=dims;++i)
                {
                    if(i!=excl)
                        my << i;
                }
            }

            ISet(const ISet &_) :
            Proxy<const IndexList>(),
            my(_.my)
            {}

            bool search(const size_t i) const noexcept
            {
                for(const IndexNode *node=my.head;node;node=node->next)
                {
                    if( i == **node ) return true;
                }
                return false;
            }


            bool atTail(const size_t i)
            {
                if(search(i)) return false;
                my << i;      return true;
            }

            bool atHead(const size_t i)
            {
                if(search(i)) return false;
                my >> i;      return true;
            }


            void shuffle(Random::Bits &ran) noexcept
            {
                Random::Shuffle::List(my,ran);
            }

            static bool AreEquivalent(const ISet &lhs, const ISet &rhs) noexcept
            {
                if(lhs->size!=rhs->size) return false;
                for(const IndexNode *node=rhs->head;node;node=node->next)
                {
                    if( !lhs.search(**node) ) return false;
                }
                return true;
            }

            virtual ~ISet() noexcept
            {
            }

        private:
            Y_DISABLE_ASSIGN(ISet);
            Y_PROXY_DECL();

            IndexList my;

        };

        Y_PROXY_IMPL(ISet,my)

        typedef Apex::Ortho::Metrics QMetrics;
        typedef Apex::Ortho::Family  QFamily;
        typedef Apex::Ortho::VCache  QVCache;

        class Tribe : public QMetrics
        {
        public:
            typedef CxxListOf<Tribe> List;




            template <typename MATRIX>
            explicit Tribe(const MATRIX    &data,
                           const size_t     indx,
                           const IndexBank &bank,
                           const QVCache   &qvcc) :
            QMetrics(data.cols),
            content(bank,indx),
            residue(bank,data.rows,indx),
            next(0),
            prev(0)
            {

                std::cerr << "using " << content << " / " << residue << std::endl;
            }

            Y_OSTREAM_PROTO(Tribe);

            template <typename MATRIX>
            explicit Tribe(const MATRIX &          data,
                           const Tribe  &          root,
                           const IndexNode * const node) :
            Metrics(root),
            content(root.content),
            residue(content->proxy),
            next(0),
            prev(0)
            {
                AddToContent(Coerce(content),**node);
                AddToResidue(Coerce(residue),node);
            }


            static inline
            void AddToContent(ISet &newContent, const size_t newIndex)
            {
                if(!newContent.atTail(newIndex)) throw Exception("Bad index");
            }

            static inline
            void AddToResidue(ISet &newResidue, const IndexNode * const node)
            {
                assert(0==newResidue->size);
                for(const IndexNode *scan=node->prev;scan;scan=scan->prev)
                {
                    if(!newResidue.atHead(**scan)) throw Exception("Bad index");
                }
                for(const IndexNode *scan=node->next;scan;scan=scan->next)
                {
                    if(!newResidue.atTail(**scan)) throw Exception("Bad index");
                }
            }


            template <typename MATRIX>
            void unfold(List &tribes, const MATRIX &data) const
            {
                // create tribe by residue promotion
                for(const IndexNode *node=residue->head;node;node=node->next)
                {
                    tribes.pushTail( new Tribe(data,*this,node) );
                }

            }

            const ISet content;
            const ISet residue;

            Tribe *next;
            Tribe *prev;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribe);
        };

        std::ostream & operator<<(std::ostream &os, const Tribe &tribe)
        {
            os << tribe.content << ":" << tribe.residue;
            return os;
        }


#if 1
        class Tribes : public Proxy<const Tribe::List>
        {
        public:

            static Apex::Natural MaxCount(const size_t rows)
            {
                Apex::Natural res;
                for(size_t i=1;i<=rows;++i) res += Apex::Natural::Arrange(rows,i);
                return res;
            }

            template <typename MATRIX> inline
            explicit Tribes(const MATRIX    &data,
                            const IndexBank &bank,
                            const QVCache   &qvcc) :
            my()
            {
                // initializing
                for(size_t i=1;i<=data.rows;++i) {
                    my.pushTail( new Tribe(data,i,bank,qvcc) );
                }

                // size: Arrange(n,1) = n

            }

            Y_OSTREAM_PROTO(Tribes);

            template <typename MATRIX>
            void generate(const MATRIX &data)
            {
                Tribe::List ng;
                for(const Tribe *tribe=my.head;tribe;tribe=tribe->next)
                {
                    tribe->unfold(ng,data);
                }
                ng.swapWith(my);
            }



            virtual ~Tribes() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribes);
            Y_PROXY_DECL();

            Tribe::List my;
        };

        Y_PROXY_IMPL(Tribes,my)

        std::ostream & operator<<(std::ostream &os, const Tribes &tribes)
        {
            if(tribes->size <= 0) os << "{}";
            else
            {
                os << '{';
                os << " #" << tribes->size;
                os << std::endl;
                for(const Tribe *tribe=tribes->head;tribe;tribe=tribe->next)
                {
                    os << "  " << *tribe << std::endl;
                }
                os << '}' << std::endl;
            }
            return os;
        }
#endif

    }
}

using namespace Yttrium;
using namespace Apex;

#include "y/container/matrix.hpp"

Y_UTEST(osprey)
{
    Random::ParkMiller ran;
    Osprey::IndexBank  bank;

    { Osprey::ISet _(bank); }

    for(size_t dims=1;dims<=5;++dims)
    {
        for(size_t nrot=0;nrot<dims;++nrot)
        {
            Osprey::ISet iset(bank,dims,nrot);
            Osprey::ISet copy(iset);
            Osprey::ISet rset(iset); rset.shuffle(ran);
            std::cerr << iset << " / " << copy << " / " << rset << std::endl;
            Y_ASSERT( Osprey::ISet::AreEquivalent(iset,copy) );
            Y_ASSERT( Osprey::ISet::AreEquivalent(iset,rset) );

        }
    }




    Matrix<int>     mu(6,7);
    Osprey::QMetrics metrics(mu.cols);
    Osprey::QVCache  vcache = new Apex::Ortho::Vector::Cache(metrics);
    Osprey::Tribes   tribes(mu,bank,vcache);

    size_t count = 0;
    while( tribes->size > 0 )
    {
        count += tribes->size;
        std::cerr << "tribes=" << tribes << std::endl;
        std::cerr << tribes->size << " / " << Natural::Arrange(mu.rows,tribes->head->content->size) << std::endl;
        tribes.generate(mu);
    }
    std::cerr << std::endl;
    std::cerr << "count(" << mu.rows << ")=" << count << " / " << Osprey::Tribes::MaxCount(mu.rows) << std::endl;


    for(size_t rows=1;rows<=10;++rows)
    {
        std::cerr << "MaxCount(" << std::setw(2) << rows << ")=" << std::setw(20) << Osprey::Tribes::MaxCount(rows) << std::endl;
    }

}
Y_UDONE()
