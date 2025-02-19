//! \file


#ifndef Y_Osprey_IProxy_Included
#define Y_Osprey_IProxy_Included 1


#include "y/data/small/heavy/list/coop.hpp"
#include "y/type/proxy.hpp"

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
            explicit IProxy(const IBank &) noexcept;
            virtual ~IProxy()              noexcept;
            IProxy(const IProxy &);

            bool tryRemove(const size_t indx) noexcept;

        protected:
            IList my; //!< my list

        private:
            Y_DISABLE_ASSIGN(IProxy);
            Y_PROXY_DECL();
        };


    }

}


#endif

