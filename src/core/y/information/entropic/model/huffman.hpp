//! \file

#ifndef Y_Information_Entropic_Huffman_Included
#define Y_Information_Entropic_Huffman_Included 1

#include "y/information/entropic/model.hpp"
#include "y/ordered/heap.hpp"
#include "y/ordered/core/compiled-raw-buffer.hpp"

namespace Yttrium
{
    namespace Information
    {

        namespace Entropic
        {

            class Huffman : public Model
            {
            public:
                static const size_t MaxUnits = Unit::MaxUsed;
                static const size_t MaxNodes = 2*MaxUnits-1;

                struct Node
                {
                    Frequency       f;
                    Node           *p;
                    Node           *l;
                    Node           *r;
                    static Node *Zeroed(Node &node) noexcept;

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


                explicit Huffman() noexcept;
                virtual ~Huffman() noexcept;
                virtual void build(Unit::List &used) noexcept;


                HandleType const root;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Huffman);
                static const size_t Required = MaxNodes * sizeof(Node);

                PQueueType       heap; //!< heap to build codes
                HandleType const knot; //!< all nodes
                void *           wksp[ Y_WORDS_GEQ(Required) ];


            };

        }

    }

}

#endif
