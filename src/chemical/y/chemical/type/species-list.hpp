
//! \file


#ifndef Y_Chemical_SpeciesList_Included
#define Y_Chemical_SpeciesList_Included 1

#include "y/chemical/reactive/components.hpp"
#include "y/check/static.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class OrderedSList : public Proxy<const SList>
        {
        public:
            explicit OrderedSList() noexcept;
            virtual ~OrderedSList() noexcept;

            static SignType Compare(const SNode * const lhs, const SNode * const rhs) noexcept;

            OrderedSList & operator<<(const Species &);
            void collect(const Components &eq);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OrderedSList);
            Y_PROXY_DECL();
            virtual void update() noexcept = 0;
        protected:
            SList list;

        };


        template <Level LEVEL>
        class SpeciesList : public OrderedSList
        {
        public:
            explicit SpeciesList() noexcept : OrderedSList() { Y_STATIC_CHECK(LEVEL!=TopLevel,BadLevel); }
            virtual ~SpeciesList() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SpeciesList);

            inline virtual void update() noexcept
            {
                size_t sub = 1;
                for(SNode *node=list.head;node;node=node->next)
                    Coerce( (**node).indx[LEVEL] ) = sub++;
            }

        };

        typedef SpeciesList<SubLevel> SubSList;

    }
}

#endif

