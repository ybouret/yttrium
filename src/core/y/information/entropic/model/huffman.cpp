
#include "y/information/entropic/model/huffman.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Information
    {

        namespace Entropic
        {

#if 1
            Huffman::Node::Comparator::  Comparator() noexcept {}
            Huffman::Node::Comparator:: ~Comparator() noexcept {}
            SignType Huffman::Node::Comparator:: operator()(const Node * const lhs, const Node * const rhs) const noexcept
            {
                return Comparison::CxxDecreasing(lhs->freq,rhs->freq);
            }

            void Huffman::Node:: make() noexcept
            {
                const unsigned childBits = bits+1;
                const uint32_t childCode = code << 1;
                
                if(left)
                {
                    left->bits = childBits;
                    left->code = childCode;
                    left->make();
                }

                if(right)
                {
                    right->bits = childBits;
                    right->code = childCode | 1;
                    right->make();
                }
            }



            Huffman:: Huffman() noexcept :
            root(0),
            heap(),
            knot(0),
            wksp()
            {
                Coerce(knot) = static_cast<HandleType>( Y_STATIC_ZARR(wksp) );
            }

            Huffman:: ~Huffman() noexcept
            {

            }
            
            template <typename T>
            static inline T * Zeroed(T &obj) noexcept
            {
                return static_cast<T *>( memset(&obj,0x00,sizeof(T)) );
            }

            void Huffman:: build(Symbol::List &used) noexcept
            {
                assert(used.size>0);

                heap.free();
                {
                    //----------------------------------------------------------
                    //
                    // loading leaves into heap
                    //
                    //----------------------------------------------------------
                    size_t      indx = 0;
                    for(Symbol *symb = used.head;symb;symb=symb->next)
                    {
                        Node *node = Zeroed<Node>(knot[indx++]);
                        node->freq = symb->freq;
                        symb->data = node;
                        heap.insert(node);
                    }

                    //----------------------------------------------------------
                    //
                    // building binary tree from heap
                    //
                    //----------------------------------------------------------
                    while(heap.size()>1)
                    {
                        Node *         left  = heap.pull(); // pull left
                        Node *         right = heap.pull(); // pull right
                        const uint32_t freq  = right->freq+left->freq;
                        Node *         node = Zeroed<Node>(knot[indx++]);
                        node->left    = left;
                        node->right   = right;
                        node->freq    = freq;
                        heap.insert(node);
                    }
                }

                //----------------------------------------------------------
                //
                // retrieve root and make recursive code
                //
                //----------------------------------------------------------
                Coerce(root) = heap.pull();
                root->make();
                for(Symbol *symb = used.head;symb;symb=symb->next)
                {
                    assert(0!=symb->data);
                    const Node *node = static_cast<const Node *>(symb->data);
                    symb->code = node->code;
                    symb->bits = node->bits;
                }

            }
#endif
            
        }

    }

}


