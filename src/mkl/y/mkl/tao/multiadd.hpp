
//! \file

#ifndef Y_MKL_TAO_Multiadd_Included
#define Y_MKL_TAO_Multiadd_Included 1

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
            class XAdd : public Object, public Antelope::Add<T>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<XAdd>  List; //!< alias
                typedef CxxPoolOf<XAdd>  Pool; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with initial capacity
                inline explicit XAdd(const size_t n) :
                Object(),
                Antelope::Add<T>(n),
                next(0),
                prev(0)
                {}

                //! cleanup
                inline virtual ~XAdd() noexcept {}

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                XAdd *next; //!< for list/pool
                XAdd *prev; //!< for list/pool

            private:
                Y_DISABLE_COPY_AND_ASSIGN(XAdd);
            };

            //__________________________________________________________________
            //
            //
            //
            //! Handling multiple eXtende Add
            //
            //
            //__________________________________________________________________
            template <typename T>
            class MultiAdd : public XAdd<T>::List
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef typename XAdd<T>::List ListType; //!< alias
                typedef typename XAdd<T>::Pool PoolType; //!< alias

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
                inline explicit MultiAdd() noexcept : ListType(), pool() {} //!< setup
                inline virtual ~MultiAdd() noexcept                      {} //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! make parallel>0, dimension for each member
                inline XAdd<T> *make(const size_t parallel, const size_t dimension)
                {
                    assert(parallel>0);

                    // adapt parallelism
                    switch( Sign::Of(size,parallel) )
                    {
                        case Negative: 
                            assert(size<parallel);
                            while(size<parallel && pool.size>0) pushTail( pool.query() );
                            while(size<parallel) pushTail( new XAdd<T>(dimension) );
                            break;
                        case __Zero__: break;
                        case Positive: assert(size>parallel);
                            while(size>parallel) pool.store( popTail() );
                    }

                    // adapt capacity
                    assert(size==parallel);
                    assert(0!=head);
                    for(XAdd<T> *node=head;node;node=node->next)
                    {
                        node->make(dimension);
                        assert(node->isEmpty());
                        assert(node->accepts(dimension));
                    }


                    return head;
                }

                //! make(1,dimension)
                inline XAdd<T> &make(const size_t dimension)
                {
                    return *make(1,dimension);
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                PoolType pool; //!< pool

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MultiAdd);
            };

        }

    }

}

#endif
