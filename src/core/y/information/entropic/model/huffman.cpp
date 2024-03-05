
#include "y/information/entropic/model/huffman.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Information
    {

        namespace Entropic
        {

            Huffman::Node::Comparator::  Comparator() noexcept {}
            Huffman::Node::Comparator:: ~Comparator() noexcept {}
            SignType Huffman::Node::Comparator:: operator()(const Node * const lhs, const Node * const rhs) const noexcept
            {
                return Comparison::CxxDecreasing(lhs->f,rhs->f);
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
                        Node *node = Zeroed<Node>(knot[indx++]);
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
                        const uint32_t f = r->f+l->f;
                        std::cerr << "  l@" << l->f << " : r@" << r->f << " => @" << f << std::endl;
                        Node *node = Zeroed<Node>(knot[indx++]);
                        node->l = l;
                        node->r = r;
                        node->f = f;
                        l->p = r->p = node;
                        heap.insert(node);
                    }
                    std::cerr << "used=" << used.size << ", indx=" << indx << std::endl;
                }

                //----------------------------------------------------------
                //
                // retrieve root
                //
                //----------------------------------------------------------
                Coerce(root) = heap.pull();


                for(Unit *unit=used.head;unit;unit=unit->next)
                {
                    assert(0!=unit->priv);
                    const Node *node = static_cast<Node *>(unit->priv);
                    unit->bits = 0;
                    unit->code = 0;
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

        }

    }

}


