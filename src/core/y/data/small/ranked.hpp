//! \file

#ifndef Y_Data_Small_Ranked_Included
#define Y_Data_Small_Ranked_Included 1

#include "y/type/proxy.hpp"
#include "y/type/signs.hpp"
#include "y/container/recyclable.hpp"

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
            typedef typename LIST::NodeType  NodeType;  //!< alias
            typedef typename LIST::Type      Type;      //!< alias
            typedef typename LIST::ConstType ConstType; //!< alias
            typedef typename LIST::ParamType ParamType; //!< alias

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
            inline explicit Ranked(ARGS &args) : Proxy<const LIST>(), list(args), compare() {}

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
            inline bool grow(ParamType args) {

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


        private:
            LIST    list;
            COMPARE compare;
            Y_DISABLE_COPY_AND_ASSIGN(Ranked);


            inline virtual
            typename Proxy<const LIST>::ConstInterface & surrogate() const noexcept
            {
                return list;
            }
        };


    }
}

#endif

