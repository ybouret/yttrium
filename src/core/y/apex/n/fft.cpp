#include "y/apex/natural.hpp"
#include "y/apex/block/factory.hpp"
#include "y/type/utils.hpp"
#include "y/dft/dft.hpp"

namespace Yttrium
{
    namespace Apex
    {


        namespace
        {
            typedef double Real;

            template <typename T>
            class BlockOf
            {
            public:
                explicit BlockOf(const size_t n) :
                block(n*sizeof(T),AsCapacity),
                size(n),
                item( static_cast<T *>(block->entry) - 1)
                {
                    assert(block->range>=n*sizeof(T));
                    assert( IsPowerOfTwo(n) );
                }

                inline T & operator[](const size_t indx) noexcept
                {
                    assert(indx>=1);
                    assert(indx<=size);
                    return item[indx];
                }

                inline T * operator()(void) noexcept { return item; }

            private:
                BlockPtr     block;
            public:
                const size_t size;
                T * const    item; // [1..size]
            private:
                Y_DISABLE_COPY_AND_ASSIGN(BlockOf);
            };
        }



        static inline
        void transferData(Real *target, const uint8_t *data, size_t size)
        {
            for(;size>0;--size) {
                *(target++) = *(data++);
                ++target;
            }
        }

        Block * Block:: DFT(Block &lhs, Block &rhs)
        {
            static Factory & factory = Factory::Instance();
            const Jig1 & a = lhs.make<Plan1>();
            const size_t n = a.words; if(n<=0) return factory.query(0);
            const Jig1 & b = rhs.make<Plan1>();
            const size_t m = b.words; if(m<=0) return factory.query(0);

            size_t       length = 1;
            {
                // find smallest power of two
                unsigned     lshift = 0;
                const size_t maxLen = Max(n,m);
                while(length<maxLen)
                {
                    length <<= 1;
                    ++lshift; // TODO: check
                }

                // next one to avoid overlap
                length <<= 1;
                ++lshift;    // TODO: check
            }

            // workspace
            const size_t  required = length << 1;
            BlockOf<Real> L(required);

            // pack
            transferData(&L[1],a.word,n);
            transferData(&L[2],b.word,m);

            std::cerr << "a=" << a << std::endl;
            std::cerr << "b=" << b << std::endl;
            Core::Display(std::cerr, &L[1], required) << std::endl;


            // transform
            Yttrium::DFT::Forward(L.item,0);
            {
                BlockOf<Real> R(required);
                Yttrium::DFT::Unpack(L.item, R.item, length);
                Yttrium::DFT::Multiply(L.item,R.item,length);
            }

            // retriev
            Yttrium::DFT::Reverse(L.item,0);
            Core::Display(std::cerr, &L[1], required) << std::endl;





            return factory.query(0);
        }
    }
}


namespace Yttrium
{
    namespace Apex
    {
        Block * Natural:: DFT(const Natural &lhs, const Natural &rhs)
        {
            Y_Apex_Lock(lhs);
            Y_Apex_Lock(rhs);
            return Block::DFT(*lhs.block,*rhs.block);
        }
    }
}
