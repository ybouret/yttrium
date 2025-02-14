
#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"

#include "y/data/small/heavy/list/coop.hpp"
#include "y/data/small/light/list/coop.hpp"

#include "y/random/shuffle.hpp"
#include "y/random/park-miller.hpp"

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
                          const size_t     dims,
                          const size_t     nrot) :
            my(ibank)
            {
                for(size_t i=1;i<=dims;++i) my << i;
                for(size_t i=nrot;i>0;--i)  my.rotl();
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

            bool insert(const size_t i)
            {
                if(search(i)) return false;
                my << i;      return true;
            }

            void shuffle(Random::Bits &ran) noexcept
            {
                Random::Shuffle::List(my,ran);
            }

            static bool AreEqual(const ISet &lhs, const ISet &rhs) noexcept
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

        class Tribe : public Object
        {
        public:
            template <typename MATRIX>
            explicit Tribe(const MATRIX    &data,
                           const size_t     rank,
                           const IndexBank &bank) :
            content(bank,data.rows,rank),
            residue(bank),
            next(0),
            prev(0)
            {
            }

            Tribe(const Tribe &_) :
            content(_.content),
            residue(_.residue),
            next(0),
            prev(0)
            {
            }




            const ISet content;
            const ISet residue;

            Tribe *next;
            Tribe *prev;
        private:
            Y_DISABLE_ASSIGN(Tribe);
        };

    }
}

using namespace Yttrium;

Y_UTEST(osprey)
{
    Random::ParkMiller ran;
    Osprey::IndexBank  ibank;

    { Osprey::ISet _(ibank); }

    for(size_t dims=1;dims<=5;++dims)
    {
        for(size_t nrot=0;nrot<dims;++nrot)
        {
            Osprey::ISet iset(ibank,dims,nrot);
            Osprey::ISet copy(iset);
            Osprey::ISet rset(iset); rset.shuffle(ran);
            std::cerr << iset << " / " << copy << " / " << rset << std::endl;
        }
    }


}
Y_UDONE()
