//! \file

#ifndef Y_Data_Small_Ranked_Included
#define Y_Data_Small_Ranked_Included 1

#include "y/type/proxy.hpp"
#include "y/type/signs.hpp"
#include "y/container/recyclable.hpp"
#include "y/type/parameter.hpp"

namespace Yttrium
{

    namespace Small
    {


        //______________________________________________________________________
        //
        //
        //
        //! Ranked List base on Small Lists
        //
        //
        //______________________________________________________________________
        template <
        typename LIST,
        typename COMPARE
        >
        class Ranked : public Proxy<const LIST>, public Recyclable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef typename LIST::NodeType      NodeType;  //!< alias
            typedef typename NodeType::Type      Type;      //!< alias
            typedef typename NodeType::ConstType ConstType; //!< alias
            typedef typename NodeType::ParamType ParamType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! default setup
            inline explicit Ranked() noexcept : Proxy<const LIST>(), list(), compare() {}

            //! setup with one argument
            template <typename ARGS>
            inline explicit Ranked(ARGS &args, const AsParameter_ &) : Proxy<const LIST>(), list(args), compare() {}

            inline Ranked(const Ranked &other) :
            Proxy<const LIST>(),
            list(other.list),
            compare() {}

            //! cleanup
            inline virtual ~Ranked() noexcept {}


            //__________________________________________________________________
            //
            //
            //  [Recyclable]
            //
            //__________________________________________________________________
            inline virtual void free() noexcept { list.free(); }

            //__________________________________________________________________
            //
            //
            //  Methods
            //
            //__________________________________________________________________

            //! master insertion algorithm
            inline bool insert(ParamType args) {

                //______________________________________________________________
                //
                //
                // check size dependence
                //
                //______________________________________________________________
                switch(list.size)
                {
                    case 0: list << args; return true;
                    case 1:
                        switch( compare(args,**list.head))
                        {
                            case __Zero__: return false;        // same value
                            case Negative: list >> args; break; // push head
                            case Positive: list << args; break; // push tail
                        }
                        return true;
                    default:
                        break;
                }
                assert(list.size>=2);

                //______________________________________________________________
                //
                //
                // check against head
                //
                //______________________________________________________________
                NodeType *lower = list.head;
                switch( compare(args,**lower) )
                {
                    case Negative: list >> args; return true; // push head
                    case __Zero__: return false;              // same value
                    case Positive: break;                     // default
                }

                //______________________________________________________________
                //
                //
                // check against tail
                //
                //______________________________________________________________
                NodeType * const upper = list.tail;
                switch(compare(args,**upper))
                {
                    case Negative: break;                     // default
                    case __Zero__: return false;              // same value
                    case Positive: list << args; return true; // push tail
                }


                NodeType * probe = lower->next;
                while(upper!=probe)
                {
                    switch(compare(args,**probe))
                    {
                        case Negative: goto INSERT_AFTER_LOWER;
                        case __Zero__: return false; // same value
                        case Positive: break;        // next probe
                    }
                    lower = probe;
                    probe = probe->next;
                }

            INSERT_AFTER_LOWER:
                list.insertAfter(lower,list.proxy->produce(args));
                return true;
            }

            //! lookup using comparator
            inline bool has(ParamType args) const noexcept
            {
                return 0 != query(args);
            }

            inline bool remove(ParamType args) noexcept
            {
                const NodeType *node = query(args);
                if(0!=node)
                {
                    list.cutNode( &Coerce(*node) );
                    return true;
                }
                else
                {
                    return false;
                }
            }

            inline void tradeWith(Ranked &other) noexcept
            {
                list.swapWith(other.list);
            }


        private:
            LIST    list;
            COMPARE compare;
            Y_DISABLE_ASSIGN(Ranked);
            inline virtual
            typename Proxy<const LIST>::ConstInterface & surrogate() const noexcept
            {
                return list;
            }

            inline const NodeType *query(ParamType args) const noexcept
            {
                const NodeType * const lower = list.head;
                switch(list.size)
                {
                    case 0: return 0;                                               // empty
                    case 1: return (__Zero__ == compare(args,**lower)) ? lower : 0; // must match sole item
                    default: break;                                                 // generic case
                }

                switch( compare(args,**lower) )
                {
                    case Negative: return 0;      // smaller than smallest
                    case __Zero__: return lower;  // match!
                    case Positive: break;         // look aboce
                }

                const NodeType * const upper = list.tail;
                switch( compare(args,**upper) )
                {
                    case Negative: break;         // look below
                    case __Zero__: return upper;  // match!
                    case Positive: return 0;      // greater than greatest
                }

                for(const NodeType *node=lower->next;upper!=node;node=node->next)
                {
                    switch( compare(args,**node) )
                    {
                        case Negative: return 0;     // no possible further match
                        case __Zero__: return node;  // match!
                        default:          continue;  // next
                    }
                }


                return 0; // not found
            }
        };


    }
}

#endif

