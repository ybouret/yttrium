//! \file



#ifndef Y_MKL_Antelope_Caddy_Included
#define Y_MKL_Antelope_Caddy_Included 1


#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {
            //__________________________________________________________________
            //
            //
            //
            //! Doubly Linked Add<T>
            //
            //
            //__________________________________________________________________
            template <typename T>
            class AddNode : public Add<T>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________

                //! compare by ability, for OrderedList
                class Comparator
                {
                public:
                    inline  Comparator() noexcept {} //!< setup
                    inline ~Comparator() noexcept {} //!< cleanup

                    //! compare abilities
                    inline SignType operator()(const AddNode *lhs, const AddNode *rhs) const noexcept
                    {
                        const size_t lhsAbility = lhs->ability();
                        const size_t rhsAbility = rhs->ability();
                        return Sign::Of(lhsAbility,rhsAbility);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Comparator);
                };

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline explicit AddNode() : Add<T>(), next(0), prev(0) {} //!< setup
                inline virtual ~AddNode() noexcept {}                     //!< cleanup

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                AddNode *next; //!< for list
                AddNode *prev; //!< for list
            private:
                Y_DISABLE_COPY_AND_ASSIGN(AddNode);

            };

            template <typename T,const bool> class CaddyProxy;

            //__________________________________________________________________
            //
            //
            //
            //! Type agnostic API
            //
            //
            //__________________________________________________________________
            class CaddyAPI
            {
            public:
                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual void setup(const size_t numVars, const size_t numData) = 0; //!< setup for vars and data
                virtual void reset(const size_t numData)                       = 0; //!< restart for all vars
                virtual void flush() noexcept                                  = 0; //!< back to pool

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~CaddyAPI() noexcept; //!< cleanup
            protected:
                explicit CaddyAPI() noexcept; //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(CaddyAPI);
            };

        }
    }

}

#include "y/data/list/ordered.hpp"


namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {

            template <typename T>
            class CaddyProxy<T,true>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef AddNode<T>                     XNode;                        //!< alias
                typedef typename XNode::Comparator     XComp;                        //!< alias
                static const OrderedListQuery          XGrab = OrderedListQueryTail; //!< alias
                typedef OrderedList<XNode,XComp,XGrab> XPool;                        //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline virtual ~CaddyProxy() noexcept {} //!< cleanup
            protected:
                inline explicit CaddyProxy() noexcept {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(CaddyProxy);
            };

        }

    }

}

#include "y/data/pool/cxx.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {

            template <typename T>
            class CaddyProxy<T,false>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef AddNode<T>       XNode; //!< alias
                typedef CxxPoolOf<XNode> XPool; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline virtual ~CaddyProxy() noexcept {} //!< cleanup
            protected:
                inline explicit CaddyProxy() noexcept {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(CaddyProxy);
            };

        }

    }

}


namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {

            template <typename T>
            class Caddy : public CaddyAPI, public CaddyProxy<T, Wary<T>::Flag >, public CxxListOf< AddNode<T> >
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const bool                IsWary = Wary<T>::Flag; //!< alias
                typedef CaddyProxy<T,IsWary>     BaseType;               //!< alias
                typedef typename BaseType::XNode XNode;                  //!< alias
                typedef CxxListOf<XNode>         XList;                  //!< alias
                typedef typename BaseType::XPool XPool;                  //!< alias
                using XList::size;
                using XList::head;
                using XList::popTail;
                using XList::pushTail;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline explicit Caddy() noexcept : XList(), pool() {} //!< setup empty
                inline virtual ~Caddy() noexcept {}                   //!< cleanup

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                inline virtual void setup(const size_t numVars,
                                          const size_t numData)
                {
                    flush();
                    try {
                        while(size<numVars) {
                            XNode *node = pushTail( (pool.size > 0) ? pool.query() : new XNode() );
                            node->make(numData);
                        }
                    }
                    catch(...) { flush(); throw; }
                }

                inline void reset(const size_t numData) {
                    for(XNode *node=head;node;node=node->next) node->make(numData);
                }

                inline virtual void flush() noexcept {
                    while(size>0) pool.store( popTail() )->free();
                }

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                template <typename ITERATOR> inline
                void sum(ITERATOR it) {
                    for(XNode *node = head;node;node=node->next,++it) *it = node->sum();
                }

                template <typename SEQUENCE> inline
                void sumForward(SEQUENCE &seq) {
                    assert(seq.size() == size);
                    sum(seq.begin());
                }

                template <typename SEQUENCE> inline
                void sumReverse(SEQUENCE &seq) {
                    assert(seq.size() == size);
                    sum(seq.rbegin());
                }




                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
            protected:
                XPool pool; //!< pool of nodes

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Caddy);
            };

        }

    }

}



#endif
