
//! \file

#ifndef Y_Sequence_Snake_Included
#define Y_Sequence_Snake_Included 1

#include "y/sequence/interface.hpp"
#include "y/container/dynamic.hpp"
#include "y/container/writable.hpp"
#include "y/type/capacity.hpp"
#include "y/data/list.hpp"
#include "y/data/pool.hpp"
#include "y/object.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/container/iterator/linked.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! Base class to hold definitions
        //
        //______________________________________________________________________
        class Snake
        {
        public: static const char * const CallSign; //!< "Snake"
        protected: explicit Snake() noexcept;        //!< setup
        public:    virtual ~Snake() noexcept;        //!< cleanup
        private:   Y_DISABLE_COPY_AND_ASSIGN(Snake);
        };
    }
    
    //__________________________________________________________________________
    //
    //
    //
    //! List of data
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class Snake :
    public Dynamic,
    public Sequence<T>,
    public Core::Snake,
    public Writable<T>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type); //!< aliases

        //______________________________________________________________________
        //
        //! internal node
        //______________________________________________________________________
        class Node
        {
        public:
            inline Node(ConstType &args) : next(0), prev(0), data(args) {} //!< setup
            inline Node() noexcept {}                                      //!< cleanup

            inline ConstType & operator*() const noexcept { return data; } //!< access
            inline Type      & operator*()       noexcept { return data; } //!< access
                                                                          
            //! zombify live node
            static inline Node *Zap(Node *node)  noexcept { return Memory::OutOfReach::Naught(node); }

            Node *next; //!< for list/pool
            Node *prev; //!< for list
        private:
            Type data;
            Y_DISABLE_COPY_AND_ASSIGN(Node);
        };

        typedef ListOf<Node> List; //!< alias
        typedef PoolOf<Node> Pool; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup for a given initial length
        inline explicit Snake(const size_t length) :
        list(), pool()
        {
            try { stash(length); } catch(...) { prune(); throw; }
        }

        //! cleanup
        inline virtual ~Snake() noexcept { erase(); }

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        inline virtual const char * callSign()             const noexcept { return CallSign;  }             //!< "Snake"
        inline virtual size_t       size()                 const noexcept { return list.size; }             //!< current body size
        inline virtual size_t       capacity()             const noexcept { return list.size + pool.size; } //!< maximal length
        inline virtual size_t       available()            const noexcept { return pool.size; }             //!< available length
        inline virtual void         free()                       noexcept { empty();   }                    //!< free body
        inline virtual void         release()                    noexcept { erase();   }                    //!< max length would be zero!!
        inline virtual void         reserve(const size_t n)               { stash(n);  }                    //!< increase max length
        inline virtual void         popHead()                    noexcept { cutHead(); }                    //!< remove head
        inline virtual void         popTail()                    noexcept { cutTail(); }                    //!< remove tail
        inline virtual void         pushTail(ParamType args)              { growTail(args); }               //!< constrained growth
        inline virtual void         pushHead(ParamType args)              { growHead(args); }               //!< constrained growth

        //! access, slow
        inline virtual Type & operator[](const size_t i) noexcept
        { return **list.fetch(i); }

        //! access, slow
        inline virtual ConstType & operator[](const size_t i) const noexcept
        { return **list.fetch(i); }

        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const List &self)
        {
            return os << self.list;
        }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! empty and append args
        inline Snake & initWith(ParamType args)
        {
            empty(); growTail(args); return *this;
        }

        //__________________________________________________________________
        //
        //
        // Iterators
        //
        //__________________________________________________________________
        typedef Iterating::Linked<Type,Node,Iterating::Forward>            Iterator;      //!< alias
        typedef Iterating::Linked<ConstType,const Node,Iterating::Forward> ConstIterator; //!< alias
        inline  Iterator      begin()       noexcept { return Iterator(list.head);      }               //!< begin
        inline  Iterator      end()         noexcept { return Iterator(0);               }               //!< end
        inline  ConstIterator begin() const noexcept { return ConstIterator(list.head); }               //!< begin, const
        inline  ConstIterator end()   const noexcept { return ConstIterator(0);          }               //!< end, const

        typedef Iterating::Linked<Type,Node,Iterating::Reverse>            ReverseIterator;      //!< alias
        typedef Iterating::Linked<ConstType,const Node,Iterating::Reverse> ConstReverseIterator; //!< alias
        inline  ReverseIterator      rbegin()       noexcept { return ReverseIterator(list.tail);      }       //!< rbegin
        inline  ReverseIterator      rend()         noexcept { return ReverseIterator(0);               }       //!< rend
        inline  ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(list.tail); }       //!< rbegin, const
        inline  ConstReverseIterator rend()   const noexcept { return ConstReverseIterator(0);          }       //!< rend, const


    private:
        Y_DISABLE_ASSIGN(Snake);
        List list;
        Pool pool;

        inline virtual ConstType &getHead() const noexcept { assert(list.size>0); return **list.head; }
        inline virtual ConstType &getTail() const noexcept { assert(list.size>0); return **list.tail; }

        void growTail(ConstType &args)
        {
            if(pool.size>0)  goto PUSH_TAIL;
            if(list.size<=0) return;
            cutHead();
        PUSH_TAIL:
            assert(pool.size>0);
            list.pushTail( make(args) );
        }

        inline   void growHead(ConstType &args)
        {
            if(pool.size>0)  goto PUSH_HEAD;
            if(list.size<=0) return;
            cutTail();
        PUSH_HEAD:
            assert(pool.size>0);
            list.pushHead( make(args) );
        }


        inline void cutHead() noexcept
        {
            pool.store( Node::Zap(list.popHead()));
        }

        inline void cutTail() noexcept
        {
            pool.store( Node::Zap(list.popTail()) );
        }



        inline Node *make(ConstType &args)
        {
            Node * node = (pool.size>0) ? pool.query() : Object::zacquire<Node>();
            try {
                return new (node) Node(args);
            }
            catch(...)
            {
                Memory::OutOfReach::Zero(node,sizeof(Node));
                pool.store(node);
                throw;
            }

        }

        inline void stash(size_t n)
        {
            while(n-- >0 ) pool.store( Object::zacquire<Node>() );
        }

        inline void empty() noexcept
        {
            while(list.size>0) pool.store( Node::Zap(list.popTail()) );
        }

        inline void prune() noexcept
        {
            while(pool.size>0) Object::zrelease( pool.query() );
        }

        inline void erase() noexcept
        {
            while(list.size>0) Object::zrelease( Node::Zap(list.popTail()) );
            prune();
        }

    };


}

#endif

