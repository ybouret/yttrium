
#include "y/utest/run.hpp"
#include "y/information/entropic/alphabet.hpp"
#include "y/ordered/heap.hpp"
#include "y/ordered/core/compiled-raw-buffer.hpp"



using namespace Yttrium;
using namespace Information;

namespace Yttrium
{
    namespace Information
    {

        namespace Entropic
        {

            class Huffman : public Model
            {
            public:
                static const size_t MaxUnits = 257;
                static const size_t MaxNodes = 600;

                struct Node
                {
                    Frequency       f;
                    Node           *p;
                    Node           *l;
                    Node           *r;
                    static Node *Zeroed(Node &node) noexcept {
                        memset(&node,0,sizeof(node));
                        return &node;
                    }

                    class Comparator
                    {
                    public:
                        Comparator()  noexcept {}
                        ~Comparator() noexcept {}
                        SignType operator()(const Node *lhs, const Node *rhs) const noexcept
                        {
                            return Comparison::CxxDecreasing(lhs->f,rhs->f);
                        }
                    private:
                        Y_DISABLE_COPY_AND_ASSIGN(Comparator);
                    };

                };

                typedef Node *                                       HandleType;
                typedef Core::CompiledRawBuffer<MaxUnits,HandleType> BufferType;
                typedef Heap<HandleType,BufferType,Node::Comparator> PQueueType;
                static const size_t Required = MaxNodes * sizeof(Node);


                explicit Huffman() : 
                root(0),
                heap(),
                knot(0),
                wksp()
                {
                    knot = static_cast<HandleType>( Y_STATIC_ZARR(wksp) );
                }

                virtual ~Huffman() noexcept {}

                virtual void build(Unit::List &used)
                {
                    root = 0;
                    heap.free();

                    size_t    indx = 0;
                    for(Unit *unit = used.head;unit;unit=unit->next)
                    {
                        Node *node = Node::Zeroed(knot[indx++]);
                        node->f    = unit->freq;
                        unit->priv = node;
                        heap.insert(node);
                    }

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
                    


                }


                HandleType root;
                PQueueType heap;
                HandleType knot;
                void      *wksp[ Y_WORDS_GEQ(Required) ];


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Huffman);

            };

        }

    }
}

#include "y/string.hpp"

Y_UTEST(info_pack)
{


    Entropic::Alphabet alpha(Entropic::Multiplex,true);
    StreamBits         io;

    alpha.write(io,'a');
    alpha.reset();
    alpha.write(io,'a');
    alpha.write(io,'a');
    alpha.reset();
    alpha.write(io,'a');
    alpha.write(io,'b');

    for(int i=0;i<256;++i)
    {
        alpha.write(io,uint8_t(i));
    }
    alpha.write(io,'a');

    alpha.reset();
    io.free();

    Y_SIZEOF(Entropic::Unit);
    Y_SIZEOF(Entropic::Alphabet);
    Y_SIZEOF(Entropic::Huffman::Node);
    Y_SIZEOF(Entropic::Huffman::BufferType);
    Y_SIZEOF(Entropic::Huffman);

    Entropic::Huffman huff;
    const String      data = "banana";
    for(size_t i=1;i<=data.size();++i)
    {
        alpha.write(io,data[i]);
    }
    alpha.display(std::cerr);

    huff.build(alpha.used);


}
Y_UDONE()
