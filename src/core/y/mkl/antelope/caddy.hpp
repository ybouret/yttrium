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
                inline explicit AddNode() : Add<T>(), next(0), prev(0), indx(0) {} //!< setup
                inline virtual ~AddNode() noexcept {}                              //!< cleanup

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                AddNode     *next; //!< for list
                AddNode     *prev; //!< for list
                const size_t indx; //!< local index
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



                /**
                 \param numVars for a number of variable/parallelism
                 \param numData dimension for each varriable
                 */
                inline virtual void setup(const size_t numVars,
                                          const size_t numData)
                {

                    try
                    {
                        switch( Sign::Of(size,numVars) )
                        {
                            case Negative: assert(size<numVars);
                                while(size<numVars && pool.size>0) addNode( pool.query() );
                                while(size<numVars)                addNode( new XNode()  );
                                break;

                            case __Zero__: assert(size==numVars);
                                break;

                            case Positive: assert(size>numVars);
                                while(size>numVars) remNode( popTail() );
                                break;
                        }
                    }
                    catch(...) { flush(); throw; }
                    assert(numVars==size);


                    for(XNode *node=head;node;node=node->next)
                        node->make(numData);


                }

                inline void reset(const size_t numData) {
                    for(XNode *node=head;node;node=node->next) node->make(numData);
                }


                inline virtual void flush() noexcept {
                    while(size>0) remNode( popTail() );
                }

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                inline XNode *make(const size_t numVars, const size_t numData) {
                    setup(numVars,numData); return head;
                }

                inline Add<T> & make(const size_t numData) {
                    return *make(1,numData);
                }




                template <typename SEQUENCE> inline
                void sum(SEQUENCE &seq) {
                    assert(seq.size() == size);
                    sumIn(seq.begin());
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

                inline void addNode( XNode *node ) noexcept
                {
                    assert(0!=node);
                    assert(0==node->indx);
                    (void) pushTail(node);
                    Coerce(node->indx) = size;
                }

                inline void remNode(XNode *node) noexcept
                {
                    assert(0!=node);
                    assert(node->indx>0);
                    node->free();
                    Coerce( pool.store(node)->indx ) = 0;
                }

                template <typename ITERATOR> inline
                void sumIn(ITERATOR it) {
                    for(XNode *node = head;node;node=node->next,++it) *it = node->sum();
                }

            };

        }

    }

}



#endif
