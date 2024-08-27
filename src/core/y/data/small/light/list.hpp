//! \file

#ifndef Y_Data_Small_Light_List_Included
#define Y_Data_Small_Light_List_Included 1

#include "y/data/small/proto/list.hpp"
#include "y/data/small/light/node.hpp"

namespace Yttrium
{

    namespace Small
    {
        //______________________________________________________________________
        //
        //
        //
        //! Light List API
        //
        //
        //______________________________________________________________________
        template <typename T, template <typename> class PROXY>
        class LightList : public ProtoList< LightNode<T>, PROXY>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T,Type);                       //!< aliases
            typedef LightNode<T>              NodeType;  //!< alias
            typedef ProtoList<NodeType,PROXY> ProtoType; //!< alias
            typedef PROXY<NodeType>           ProxyType; //!< alias
            using ProtoType::proxy;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! setup default
            inline explicit LightList() : ProtoType() {}

            //! setup with proxy
            inline explicit LightList(const ProxyType &_) noexcept : ProtoType(_) {}

            //! copy
            inline explicit LightList(const LightList &_) : ProtoType(_) {}

        public:
            // cleanup
            inline virtual ~LightList() noexcept {}

            //__________________________________________________________________
            //
            //
            // API
            //
            //__________________________________________________________________

            //! pushTail
            inline LightList & operator<<( Type &args )
            {
                this->pushTail(proxy->produce(args));
                return *this;
            }

            //! mergeTail another list
            inline LightList & operator<<( const LightList &other )
            {
                LightList temp(other);
                this->mergeTail(temp);
                return *this;
            }

            //! pushHead
            inline LightList & operator>>( Type &args )
            {
                this->pushHead(proxy->produce(args));
                return *this;
            }

            //! pullTail
            inline Type & pullTail() noexcept
            {
                assert(this->size>0);
                assert(0!=this->tail);
                Type &ref = **(this->tail);
                this->cutTail();
                return ref;
            }

            //! pullHead
            inline Type & pullHead() noexcept
            {
                assert(this->size>0);
                assert(0!=this->head);
                Type &ref = **(this->head);
                this->cutHead();
                return ref;
            }

            //! sequential look up
            inline bool has(ConstType &obj) const noexcept
            {
                for(const NodeType *node=this->head;node;node=node->next)
                {
                    ConstType &ref = **node;
                    if( &ref == &obj ) return true;
                }
                return false;
            }

            //! sequential look up of all nodes
            inline bool contains(const LightList &other) const noexcept
            {
                for(const NodeType *node=other.head;node;node=node->next)
                {
                    if( !has(**node) ) return false;
                }
                return true;
            }

            //! same content
            inline bool alike(const LightList &rhs) const noexcept
            {
                const LightList &lhs = *this;
                return (lhs.size == rhs.size) && lhs.contains(rhs) && rhs.contains(lhs);
            }




            //! remove multiple references
            inline void noMultiple() noexcept
            {
                ListOf<NodeType> tmp;
                while(this->size>0)
                {
                    NodeType *  node  = this->tail;
                    ConstType & lhs   = **node;
                    bool        found = false;

                    for(const NodeType *scan=tmp.head;scan;scan=scan->next)
                    {
                        ConstType & rhs = **scan;
                        if( &lhs == &rhs )
                        {
                            found = true;
                            break;
                        }
                    }
                    if(found)
                    {
                        this->cutTail();
                    }
                    else
                    {
                        tmp.pushHead( this->popTail() );
                    }
                }
                tmp.swapWith(*this);
            }



        private:
            Y_DISABLE_ASSIGN(LightList);
        };
    }

}

#endif

