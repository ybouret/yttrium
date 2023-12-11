
//! \file

#ifndef Y_MKL_TAO_Multifold_Included
#define Y_MKL_TAO_Multifold_Included 1

#include "y/mkl/antelope/add.hpp"
#include "y/data/list/cxx.hpp"
#include "y/data/pool/cxx.hpp"

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
                typedef CxxPoolOf<DynamicAdd>  Pool; //!< alias

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
                typedef DynamicAdd<T>                NodeType; //!< alias
                typedef typename DynamicAdd<T>::List ListType; //!< alias
                typedef typename DynamicAdd<T>::Pool PoolType; //!< alias
                typedef Antelope::Add<T>             XAddType; //!< alias

                using ListType::head;
                using ListType::tail;
                using ListType::size;
                using ListType::popTail;
                using ListType::pushTail;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline explicit Multifold() noexcept : ListType(), pool() {} //!< setup
                inline virtual ~Multifold() noexcept                      {} //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! make parallel>0, dimension for each member
                inline NodeType *make(const size_t parallel, const size_t dimension)
                {
                    assert(parallel>0);

                    // adapt parallelism
                    switch( Sign::Of(size,parallel) )
                    {
                        case Negative: 
                            assert(size<parallel);
                            while(size<parallel && pool.size>0) pushTail( pool.query() );
                            while(size<parallel) pushTail( new NodeType(dimension) );
                            break;
                        case __Zero__: break;
                        case Positive: assert(size>parallel);
                            while(size>parallel) pool.store( popTail() );
                    }

                    // adapt capacity
                    assert(size==parallel);
                    for(NodeType *node=head;node;node=node->next)
                    {
                        node->make(dimension);
                    }

                    return head;
                }

                //! make(1,dimension)
                inline NodeType *make(const size_t dimension)
                {
                    return make(1,dimension);
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                PoolType pool; //!< pool

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Multifold);
            };

        }

    }

}

#endif
