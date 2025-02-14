
#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"

#include "y/data/small/heavy/list/coop.hpp"
#include "y/data/small/light/list/coop.hpp"



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

            virtual ~ISet() noexcept
            {
            }

        private:
            Y_DISABLE_ASSIGN(ISet);
            Y_PROXY_DECL();

            IndexList my;

        };

        Y_PROXY_IMPL(ISet,my)
    }
}

using namespace Yttrium;

Y_UTEST(osprey)
{
    Osprey::IndexBank ibank;

    { Osprey::ISet _(ibank); }

    for(size_t dims=1;dims<=5;++dims)
    {
        for(size_t nrot=0;nrot<dims;++nrot)
        {
            Osprey::ISet iset(ibank,dims,nrot);
            std::cerr << iset << std::endl;
        }
    }


}
Y_UDONE()
