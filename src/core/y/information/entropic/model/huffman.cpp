
#include "y/information/entropic/model/huffman.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Information
    {

        namespace Entropic
        {

#if 0
            Huffman::Node * Huffman::Node:: Zeroed(Node &node) noexcept {
                memset(&node,0,sizeof(node));
                return &node;
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
            
            void Huffman:: build(Unit::List &used) noexcept
            {
                assert(used.size>0);

                heap.free();
                {
                    //----------------------------------------------------------
                    //
                    // loading leaves into heap
                    //
                    //----------------------------------------------------------
                    size_t    indx = 0;
                    for(Unit *unit = used.head;unit;unit=unit->next)
                    {
                        Node *node = Node::Zeroed(knot[indx++]);
                        node->f    = unit->freq;
                        unit->priv = node;
                        heap.insert(node);
                    }

                    //----------------------------------------------------------
                    //
                    // building binary tree from heap
                    //
                    //----------------------------------------------------------
                    while(heap.size()>1)
                    {
                        Node *r = heap.pull();
                        Node *l = heap.pull();
                        const Frequency f = r->f+l->f;
                        std::cerr << "l@" << l->f << " : r@" << r->f << " => @" << f << std::endl;
                        Node *node = Node::Zeroed(knot[indx++]);
                        node->l = l;
                        node->r = r;
                        node->f = f;
                        l->p = r->p = node;
                        heap.insert(node);
                    }
                    std::cerr << "used=" << used.size << ", indx=" << indx << std::endl;
                }

                Coerce(root) = heap.pull();
                for(Unit *unit=used.head;unit;unit=unit->next)
                {
                    assert(0!=unit->priv);
                    const Node *node = static_cast<Node *>(unit->priv);
                    unit->bits = 0;
                UP:
                    const Node *parent = node->p;
                    if(parent)
                    {
                        unit->bits++;
                        node = parent;
                        goto UP;
                    }
                }


            }
#endif
            
        }

    }

}


