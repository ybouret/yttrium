
#include "y/hashing/mph.hpp"
#include "y/data/list/cxx.hpp"
#include "y/object.hpp"
#include "y/type/nullify.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Hashing
    {

        class MinimalPerfect:: Node : public Object
        {
        public:
            typedef CxxListOf<Node> List;

            inline explicit Node() noexcept : code(-1), byte(0), chld(), next(0), prev(0)
            {
            }

            inline virtual ~Node() noexcept {}



            const int     code;
            const uint8_t byte;
            List          chld;
            Node *        next;
            Node *        prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Node);
        };

        MinimalPerfect:: MinimalPerfect() : root( new Node() )
        {
        }

        MinimalPerfect:: ~MinimalPerfect() noexcept
        {
            assert(0!=root);
            Nullify(root);
        }

        static const char CallSign[] = "Hashing::MinimalPerfect";

        MinimalPerfect & MinimalPerfect:: operator()(const Memory::ReadOnlyBuffer &buffer,
                                                     const int                     hvalue)
        {
            assert(0!=root);
            if(hvalue<0) throw Specific::Exception(CallSign,"inserting invalid hash=%d",hvalue);

            const uint8_t * path = static_cast<const uint8_t *>( buffer.ro_addr() );
            size_t          size = buffer.measure();
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
            if(curr->code>=0)
                throw Specific::Exception(CallSign,"inserting multiple buffer with hash=%d",hvalue);

            Coerce(curr->code) = hvalue;
            return *this;
        }


    }

}
