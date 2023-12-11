
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

            //__________________________________________________________________
            //
            //
            //
            //! Allocated Add<T>
            //
            //
            //__________________________________________________________________
            template <typename T>
            class DynamicAdd : public Object, public Antelope::Add<T>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Antelope::Add<T>       XAdd; //!< eXtended addision
                typedef CxxListOf<DynamicAdd>  List; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with initial capacity
                inline explicit DynamicAdd(const size_t n) :
                Object(),
                XAdd(n),
                next(0),
                prev(0)
                {}

                //! cleanup
                inline virtual ~DynamicAdd() noexcept {}

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                DynamicAdd *next; //!< for list/pool
                DynamicAdd *prev; //!< for list/pool

            private:
                Y_DISABLE_COPY_AND_ASSIGN(DynamicAdd);
            };

            //__________________________________________________________________
            //
            //
            //
            //! Handling
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Multifold : public DynamicAdd<T>::List
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef DynamicAdd<T>                XNode; //!< alias
                typedef typename DynamicAdd<T>::List XList; //!< alias
                using XList::head;
                using XList::tail;
                using XList::size;
                using XList::popTail;
                using XList::pushTail;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline explicit Multifold() noexcept : XList(), pool() {} //!< setup
                inline virtual ~Multifold() noexcept                   {} //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! make parallel>0, dimension for each member
                inline XNode *make(const size_t parallel, const size_t dimension)
                {
                    assert(parallel>0);

                    // adapt parallelism
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

                    // adapt capacity
                    assert(size==parallel);
                    for(XNode *node=head;node;node=node->next)
                    {
                        node->make(dimension);
                    }

                    return head;
                }

                //! make(1,dimension)
                inline XNode *make(const size_t dimension)
                {
                    return make(1,dimension);
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                XList pool; //!< pool

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Multifold);
            };

        }

    }

}

#endif
