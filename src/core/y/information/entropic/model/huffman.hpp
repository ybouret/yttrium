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

            //__________________________________________________________________
            //
            //
            //
            //! Huffman Codec
            //
            //
            //__________________________________________________________________
            class Huffman : public Model
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const size_t   MaxUnits = Unit::MaxAlive; //!< max alive units
                static const size_t   MaxNodes = 2*MaxUnits-1;   //!< full binary tree
                static const uint32_t AtLeft   = 0x00;
                static const uint32_t AtRight  = 0x01;
                
                //______________________________________________________________
                //
                //
                //! node for tree
                //
                //______________________________________________________________
                struct Node
                {
                    uint32_t        f; //!< frequency
                    Node           *p; //!< parent
                    Node           *l; //!< left
                    Node           *r; //!< right


                    
                    //__________________________________________________________
                    //
                    //! comparator for heap
                    //__________________________________________________________
                    class Comparator
                    {
                    public:
                        Comparator()  noexcept; //!< setup
                        ~Comparator() noexcept; //!< cleanup

                        //! decreasing frequency
                        SignType operator()(const Node * const lhs, const Node * const rhs) const noexcept;
                    private:
                        Y_DISABLE_COPY_AND_ASSIGN(Comparator);
                    };
                };

                typedef Node *                                       HandleType; //!< alias
                typedef Core::CompiledRawBuffer<MaxUnits,HandleType> BufferType; //!< alias
                typedef Heap<HandleType,BufferType,Node::Comparator> PQueueType; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Huffman() noexcept; //!< setup
                virtual ~Huffman() noexcept; //!< cleanup

                //! build from used, used.size>0
                virtual void build(Unit::List &used) noexcept;


                HandleType const root; //!< root node
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Huffman);
                static const size_t Required = MaxNodes * sizeof(Node);

                PQueueType       heap; //!< heap to build codes
                HandleType const knot; //!< all nodes
                void *           wksp[ Y_WORDS_GEQ(Required) ]; //!< workspace


            };

        }

    }

}

#endif
