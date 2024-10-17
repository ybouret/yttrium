
#include "y/hashing/perfect.hpp"
#include "y/data/list/cxx.hpp"
#include "y/object.hpp"
#include "y/type/nullify.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Hashing
    {

        class Perfect:: Node : public Object
        {
        public:
            typedef CxxListOf<Node> List;

            inline explicit Node() noexcept : hash(-1), byte(0), chld(), next(0), prev(0)
            {
            }

            inline virtual ~Node() noexcept {}



            const int     hash;
            const uint8_t byte;
            List          chld;
            Node *        next;
            Node *        prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Node);
        };

        Perfect:: Perfect() : root( new Node() )
        {
        }


        Perfect:: Perfect(const char  *argv[],
                          const size_t argc) :
        root( new Node() )
        {
            assert(Good(argv,argc));
            Perfect &self = *this;
            for(size_t i=0;i<argc;++i) {
                self(argv[i],int(i));
            }
        }

        Perfect:: ~Perfect() noexcept
        {
            assert(0!=root);
            Nullify(root);
        }

        static const char CallSign[] = "Hashing::Perfect";

        Perfect & Perfect:: operator()(const void *const data,
                                       size_t            size,
                                       const int         hash)
        {
            assert(Good(data,size));
            assert(0!=root);
            if(hash<0) throw Specific::Exception(CallSign,"inserting invalid hash=%d",hash);

            const uint8_t * path = static_cast<const uint8_t *>( data);
            Node *          curr = root;

            //------------------------------------------------------------------
            //
            // walking the tree
            //
            //------------------------------------------------------------------
            while(size-- > 0 )
            {
                const uint8_t byte = *(path++);
                bool          flag = false;

                //--------------------------------------------------------------
                //
                // loop over children to find path
                //
                //--------------------------------------------------------------
                for(Node *node=curr->chld.head;node;node=node->next)
                {
                    if(byte==node->byte)
                    {
                        curr = node;
                        flag = true;
                        break;
                    }
                }


                if(flag)
                {
                    //----------------------------------------------------------
                    // found byte, continue to walk
                    //----------------------------------------------------------
                    assert( byte == curr->byte );
                    continue;
                }

                //--------------------------------------------------------------
                //
                // create a new child with byte and invalid code
                //
                //--------------------------------------------------------------
                curr = curr->chld.pushTail( new Node() );
                Coerce(curr->byte) = byte;
            }

            //------------------------------------------------------------------
            //
            // try to set current value
            //
            //------------------------------------------------------------------
            assert(0!=curr);
            if(curr->hash>=0)
                throw Specific::Exception(CallSign,"inserting multiple buffer with hash=%d",hash);

            Coerce(curr->hash) = hash;
            return *this;
        }


        Perfect & Perfect:: operator()(const Memory::ReadOnlyBuffer &buff, const int hash)
        {
            return (*this)(buff.ro_addr(),buff.measure(),hash);
        }

        Perfect & Perfect:: operator()(const char * const text, const int hash)
        {
            return (*this)(text,text?strlen(text):0,hash);
        }


        int Perfect:: operator()(const void * const data, size_t size) const noexcept
        {
            assert( Good(data,size) );
            const uint8_t * path = static_cast<const uint8_t *>( data);
            const Node *    curr = root;

            while(size-- > 0)
            {
                const uint8_t byte = *(path++);
                bool          flag = false;
                for(const Node *node=curr->chld.head;node;node=node->next)
                {
                    if(byte==node->byte)
                    {
                        curr = node;
                        flag = true;
                        break;
                    }
                }
                if(!flag) return -1;
            }

            assert(0!=curr);
            return curr->hash;
        }

        int Perfect:: operator()(const Memory::ReadOnlyBuffer &  buff) const noexcept
        {
            return (*this)( buff.ro_addr(), buff.measure() );
        }

        int Perfect:: operator()(const char * const              text) const noexcept
        {
            return (*this)( text, text?strlen(text):0 );
        }


    }

}
