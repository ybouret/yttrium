
//! \file

#ifndef Y_MKL_TAO_Multifold_Included
#define Y_MKL_TAO_Multifold_Included 1

#include "y/mkl/antelope/add.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {

            template <typename T>
            class DynamicAdd : public Object, public Antelope::Add<T>
            {
            public:
                typedef Antelope::Add<T>       XAdd;
                typedef CxxListOf<DynamicAdd>  List;

                inline explicit DynamicAdd(const size_t n) :
                Object(),
                XAdd(n),
                next(0),
                prev(0)
                {}

                inline virtual ~DynamicAdd() noexcept {}

                DynamicAdd *next;
                DynamicAdd *prev;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(DynamicAdd);
            };

            template <typename T>
            class Multifold : public DynamicAdd<T>::List
            {
            public:
                typedef DynamicAdd<T>                XNode;
                typedef typename DynamicAdd<T>::List XList;
                using XList::head;
                using XList::tail;
                using XList::size;
                using XList::popTail;
                using XList::pushTail;

                inline explicit Multifold() noexcept : XList(), pool() {}
                inline virtual ~Multifold() noexcept                   {}

                inline XNode *setup(const size_t parallel, const size_t dimension)
                {
                    assert(parallel>0);

                    switch( Sign::Of(size,parallel) )
                    {
                        case Negative: 
                            assert(size<parallel);
                            while(size<parallel && pool.size>0) pushTail( pool.popHead() );
                            while(size<parallel) pushTail( new XNode(dimension) );
                            break;
                        case __Zero__: break;
                        case Positive: assert(size>parallel);
                            while(size>parallel) pool.pushHead( popTail() );
                    }

                    assert(size==parallel);

                    return 0;
                }

                inline XNode *setup(const size_t dimension)
                {
                    return setup(1,dimension);
                }

                XList pool;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Multifold);
            };

        }

    }

}

#endif
